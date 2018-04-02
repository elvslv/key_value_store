#include "Node.h"
#include "CreateRequestMessage.h"
#include "utils/Exceptions.h"
#include <array>
#include <exception>
#include <functional>
#include <future>
#include <thread>

namespace key_value_store
{
Node::Node(const network::Address& address,
    const std::shared_ptr<utils::Log>& logger,
    std::unique_ptr<membership_protocol::IMembershipProtocol>
        membershipProtocol,
    std::unique_ptr<IStorage> storage,
    std::unique_ptr<IPartitioner> partitioner,
    const std::shared_ptr<utils::MessageDispatcher<RequestMessage>>& messageDispatcher)
    : address(address)
    , logger(logger)
    , membershipProtocol(std::move(membershipProtocol))
    , storage(std::move(storage))
    , partitioner(std::move(partitioner))
    , messageDispatcher(messageDispatcher)
    , runnable([this]() { run(); })
{
}

void Node::start()
{
    logger->log(address, "Starting node");
    membershipProtocol->start();

    runnable.start();
}

void Node::stop()
{
    runnable.stop();
    membershipProtocol->stop();

    logger->log(address, "Stopped node");
}

bool Node::performActionAndWait(std::function<bool()> func, std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv)
{
    bool result;
    try
    {
        result = func();
    }
    catch (...)
    {
        ++count;
        cv.notify_one();
        throw;
    }

    ++succeeded;
    ++count;
    cv.notify_one();
    return result;
}

void Node::processClientRequest(const std::string& key, std::function<bool()> localFunc, std::function<bool(const network::Address&)> remoteFunc)
{
    auto nodes = getTargetNodes(key);
    if (nodes.size() < 3)
    {
        throw std::logic_error("Not enough nodes!!!");
    }

    std::atomic<int> count(0);
    std::atomic<int> succeeded(0);

    std::mutex mutex;
    std::condition_variable cv;

    std::array<std::future<bool>, 3> futures;
    for (int i = 0; i < nodes.size(); ++i)
    {
        auto node = nodes[i];
        if (node == address)
        {
            futures[i] = std::async(std::launch::async, [this, localFunc](std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv) {
                return performActionAndWait([this, localFunc]() -> bool { return localFunc(); }, count, succeeded, cv);
            },
                std::ref(count), std::ref(succeeded), std::ref(cv));
        }
        else
        {
            futures[i] = std::async(std::launch::async, [this, remoteFunc, node](std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv) {
                return performActionAndWait([this, remoteFunc, node]() -> bool { return remoteFunc(node); }, count, succeeded, cv);
            },
                std::ref(count), std::ref(succeeded), std::ref(cv));
        }
    }

    {
        using namespace std::chrono_literals;
        std::unique_lock<std::mutex> lock(mutex);

        // TODO: add a const
        cv.wait_for(lock, 2s, [&count, &succeeded]() -> bool { return count == 3 || succeeded == 2; });
    }

    std::unique_ptr<utils::ComplexException> eptr = std::make_unique<utils::ComplexException>();
    for (auto it = futures.begin(); it != futures.end(); ++it)
    {
        try
        {
            it->get();
        }
        catch (...)
        {
            eptr->addException(std::current_exception());
        }
    }

    if (succeeded < 2)
    {
        auto ex = eptr.release();
        throw ex;
    }
}

void Node::create(const std::string& key, const std::string& value)
{
    processClientRequest(key, [this, key, value]() -> bool { return createLocally(key, value); }, [this, key, value](const network::Address& node) -> bool { return sendCreateMessage(node, key, value); });
}

bool Node::createLocally(const std::string& key, const std::string& value)
{
    storage->insert(key, Record(value));
    return true;
}

bool Node::sendCreateMessage(const network::Address& target,
    const std::string& key, const std::string& value)
{
    messageDispatcher->sendMessage(std::make_unique<CreateRequestMessage>(address, target, key, value), target);
}

std::string Node::read(const std::string& key)
{
    throw utils::NotImplementedException();
}

void Node::update(const std::string& key, const std::string& value)
{
    throw utils::NotImplementedException();
}

void Node::remove(const std::string& key)
{
    throw utils::NotImplementedException();
}

void Node::run()
{
    logger->log(address, "[Node::run] -- start");

    while (runnable.isRunning)
    {
    }

    logger->log(address, "[Node::run] -- end");
}

std::vector<network::Address> Node::getTargetNodes(const std::string& key)
{
    auto nodes = membershipProtocol->getMembers();
    nodes.push_back(membership_protocol::Member(address));

    return partitioner->getTargetNodes(key, nodes);
}
} // namespace key_value_store
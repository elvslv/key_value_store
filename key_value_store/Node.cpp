#include "Node.h"
#include "CreateRequestMessage.h"
#include "ReadRequestMessage.h"
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

void Node::create(const std::string& key, const std::string& value)
{
    processClientRequest<bool>(key, [this, key, value]() -> bool { return createLocally(key, value); }, [this, key, value](const network::Address& node) -> bool { return sendCreateMessage(node, key, value); });
}

bool Node::createLocally(const std::string& key, const std::string& value)
{
    storage->insert(key, Record(value));
    return true;
}

bool Node::sendCreateMessage(const network::Address& target, const std::string& key, const std::string& value)
{
    messageDispatcher->sendMessage(std::make_unique<CreateRequestMessage>(address, target, key, value), target);
    throw utils::NotImplementedException();
}

std::string Node::read(const std::string& key)
{
    Record result = processClientRequest<Record>(key, [this, key]() -> Record { return readLocally(key); }, [this, key](const network::Address& node) -> Record { return sendReadMessage(node, key); });
    return result.value;
}

Record Node::readLocally(const std::string& key)
{
    auto result = storage->get(key);
    return result;
}

Record Node::sendReadMessage(const network::Address& target, const std::string& key)
{
    messageDispatcher->sendMessage(std::make_unique<ReadRequestMessage>(address, target, key), target);
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
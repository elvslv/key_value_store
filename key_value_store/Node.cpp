#include "Node.h"
#include "CreateRequestMessage.h"
#include "CreateResponseMessage.h"
#include "DeleteRequestMessage.h"
#include "DeleteResponseMessage.h"
#include "ReadRequestMessage.h"
#include "ReadResponseMessage.h"
#include "UpdateRequestMessage.h"
#include "UpdateResponseMessage.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"
#include <array>
#include <exception>
#include <functional>
#include <future>
#include <thread>

namespace key_value_store
{
using namespace std::chrono_literals;

Node::Node(const network::Address& address,
    std::shared_ptr<utils::Log> logger,
    std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol,
    std::unique_ptr<IStorage> storage,
    std::unique_ptr<IPartitioner> partitioner,
    const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher,
    std::shared_ptr<utils::IThreadPolicy> threadPolicy)
    : address(address)
    , logger(logger)
    , membershipProtocol(std::move(membershipProtocol))
    , storage(std::move(storage))
    , partitioner(std::move(partitioner))
    , messageDispatcher(messageDispatcher)
    , asyncQueue(std::bind(&Node::processMessage, this, std::placeholders::_1))
    , asyncQueueCallback([this](std::unique_ptr<Message> message) { asyncQueue.push(std::move(message)); })
    , sentMessages()
    , runnable([this]() { run(); })
    , threadPolicy(std::move(threadPolicy))
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
    auto message = std::make_unique<CreateRequestMessage>(address, target, key, value);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto createResponse = static_cast<CreateResponseMessage*>(response.get());
    return createResponse->getResponseCode() == 200;
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
    auto message = std::make_unique<ReadRequestMessage>(address, target, key);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        throw std::logic_error("TBD");
    }

    auto readResponse = static_cast<ReadResponseMessage*>(response.get());
    if (response->getResponseCode() != 200)
    {
        throw std::logic_error("TBD");
    }

    return Record(readResponse->getValue());
}

void Node::update(const std::string& key, const std::string& value)
{
    processClientRequest<bool>(key, [this, key, value]() -> bool { return updateLocally(key, value); }, [this, key, value](const network::Address& node) -> bool { return sendUpdateMessage(node, key, value); });
}

bool Node::updateLocally(const std::string& key, const std::string& value)
{
    storage->update(key, Record(value));
    return true;
}

bool Node::sendUpdateMessage(const network::Address& target, const std::string& key, const std::string& value)
{
    auto message = std::make_unique<UpdateRequestMessage>(address, target, key, value);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto updateResponse = static_cast<UpdateResponseMessage*>(response.get());
    return updateResponse->getResponseCode() == 200;
}

void Node::remove(const std::string& key)
{
    processClientRequest<bool>(key, [this, key]() -> bool { return removeLocally(key); }, [this, key](const network::Address& node) -> bool { return sendRemoveMessage(node, key); });
}

bool Node::removeLocally(const std::string& key)
{
    storage->remove(key);
    return true;
}

bool Node::sendRemoveMessage(const network::Address& target, const std::string& key)
{
    auto message = std::make_unique<DeleteRequestMessage>(address, target, key);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto deleteResponse = static_cast<DeleteResponseMessage*>(response.get());
    return deleteResponse->getResponseCode() == 200;
}

void Node::run()
{
    logger->log(address, "[Node::run] -- start");

    while (runnable.isRunning)
    {
        runStabilizationProtocol();
        threadPolicy->sleepMilliseconds(1000);
    }

    logger->log(address, "[Node::run] -- end");
}

std::vector<network::Address> Node::getTargetNodes(const std::string& key)
{
    auto nodes = membershipProtocol->getMembers();
    nodes.push_back(membership_protocol::Member(address));

    return partitioner->getTargetNodes(key, nodes);
}

void Node::onCreateRequest(std::unique_ptr<CreateRequestMessage> message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->insert(message->getKey(), Record(message->getValue()));
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onUpdateRequest(std::unique_ptr<UpdateRequestMessage> message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->update(message->getKey(), Record(message->getValue()));
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onReadRequest(std::unique_ptr<ReadRequestMessage> message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    auto record = storage->get(message->getKey());
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), 200, record.value), message->getSourceAddress());
}

void Node::onDeleteRequest(std::unique_ptr<DeleteRequestMessage> message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->remove(message->getKey());
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onResponse(std::unique_ptr<ResponseMessage> message)
{
    auto messageId = message->getId();
    std::unique_lock<std::mutex> lock(mutex);
    assert(sentMessages.find(messageId) != sentMessages.end());

    sentMessages[messageId]->responseMessage.reset(message.release());
}

void Node::processMessage(std::unique_ptr<Message> message)
{
    switch (message->getMessageType())
    {
    case Message::CREATE_REQUEST:
    {
        std::unique_ptr<CreateRequestMessage> createRequestMessage(dynamic_cast<CreateRequestMessage*>(message.release()));
        onCreateRequest(std::move(createRequestMessage));
        break;
    }
    case Message::UPDATE_REQUEST:
    {
        std::unique_ptr<UpdateRequestMessage> updateRequestMessage(dynamic_cast<UpdateRequestMessage*>(message.release()));
        onUpdateRequest(std::move(updateRequestMessage));
        break;
    }
    case Message::READ_REQUEST:
    {
        std::unique_ptr<ReadRequestMessage> readRequestMessage(dynamic_cast<ReadRequestMessage*>(message.release()));
        onReadRequest(std::move(readRequestMessage));
        break;
    }
    case Message::DELETE_REQUEST:
    {
        std::unique_ptr<DeleteRequestMessage> deleteRequestMessage(dynamic_cast<DeleteRequestMessage*>(message.release()));
        onDeleteRequest(std::move(deleteRequestMessage));
        break;
    }

    case Message::CREATE_RESPONSE:
    case Message::UPDATE_RESPONSE:
    case Message::READ_RESPONSE:
    case Message::DELETE_RESPONSE:
    {
        std::unique_ptr<ResponseMessage> responseMessage(dynamic_cast<ResponseMessage*>(message.release()));
        onResponse(std::move(responseMessage));
        break;
    }
    }

    throw utils::NotImplementedException();
}

void Node::runStabilizationProtocol()
{
    auto records = storage->getRecords();
    for (auto it = records.begin(); it != records.end(); ++it)
    {
        auto nodes = getTargetNodes(it->first);
        auto curNodeIt = std::find_if(nodes.begin(), nodes.end(), [this](const network::Address& addr) -> bool { return addr == address; });
        if (curNodeIt == nodes.end())
        {
            for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
            {
                sendCreateMessage(*nodeIt, it->first, it->second.value);
            }

            continue;
        }

        int index = curNodeIt - nodes.begin();
        for (int i = 0; i < 3; ++i)
        {
            if (i == index)
            {
                continue;
            }

            if (nodes.size() > i && !isNodeAlive(nodes[i]))
            {
                sendCreateMessage(nodes[i], it->first, it->second.value);
            }
        }
    }
}

bool Node::isNodeAlive(const network::Address& addr)
{
    auto nodes = membershipProtocol->getMembers();
    auto it = std::find_if(nodes.begin(), nodes.end(), [addr](const membership_protocol::Member& member) -> bool { return member.address == addr; });
    return it != nodes.end();
}

} // namespace key_value_store
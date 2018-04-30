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
    const std::shared_ptr<utils::Log>& logger,
    std::unique_ptr<membership_protocol::IMembershipProtocol>
        membershipProtocol,
    std::unique_ptr<IStorage> storage,
    std::unique_ptr<IPartitioner> partitioner,
    const std::shared_ptr<utils::MessageDispatcher<Message>>& messageDispatcher)
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
    }

    logger->log(address, "[Node::run] -- end");
}

std::vector<network::Address> Node::getTargetNodes(const std::string& key)
{
    auto nodes = membershipProtocol->getMembers();
    nodes.push_back(membership_protocol::Member(address));

    return partitioner->getTargetNodes(key, nodes);
}

void Node::onCreateRequest(CreateRequestMessage* message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->insert(message->getKey(), Record(message->getValue()));
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onUpdateRequest(UpdateRequestMessage* message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->update(message->getKey(), Record(message->getValue()));
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onReadRequest(ReadRequestMessage* message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    auto record = storage->get(message->getKey());
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), 200, record.value), message->getSourceAddress());
}

void Node::onDeleteRequest(DeleteRequestMessage* message)
{
    auto key = message->getKey();

    // TODO: make sure it belongs to this node
    storage->remove(message->getKey());
    // TODO: handle exceptions

    messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), 200), message->getSourceAddress());
}

void Node::onCreateResponse(CreateResponseMessage* message)
{
    throw utils::NotImplementedException();
}

void Node::onUpdateResponse(UpdateResponseMessage* message)
{
    throw utils::NotImplementedException();
}

void Node::onReadResponse(ReadResponseMessage* message)
{
    throw utils::NotImplementedException();
}

void Node::onDeleteResponse(DeleteResponseMessage* message)
{
    throw utils::NotImplementedException();
}

void Node::processMessage(const std::unique_ptr<Message>& message)
{
    switch (message->getMessageType())
    {
    case Message::CREATE_REQUEST:
    {
        auto createRequestMessage = dynamic_cast<CreateRequestMessage*>(message.get());
        onCreateRequest(createRequestMessage);
        break;
    }
    case Message::UPDATE_REQUEST:
    {
        auto updateRequestMessage = dynamic_cast<UpdateRequestMessage*>(message.get());
        onUpdateRequest(updateRequestMessage);
        break;
    }
    case Message::READ_REQUEST:
    {
        auto readRequestMessage = dynamic_cast<ReadRequestMessage*>(message.get());
        onReadRequest(readRequestMessage);
        break;
    }
    case Message::DELETE_REQUEST:
    {
        auto deleteRequestMessage = dynamic_cast<DeleteRequestMessage*>(message.get());
        onDeleteRequest(deleteRequestMessage);
        break;
    }
    case Message::CREATE_RESPONSE:
    {
        auto createResponseMessage = dynamic_cast<CreateResponseMessage*>(message.get());
        onCreateResponse(createResponseMessage);
        break;
    }
    case Message::UPDATE_RESPONSE:
    {
        auto updateResponseMessage = dynamic_cast<UpdateResponseMessage*>(message.get());
        onUpdateResponse(updateResponseMessage);
        break;
    }
    case Message::READ_RESPONSE:
    {
        auto readResponseMessage = dynamic_cast<ReadResponseMessage*>(message.get());
        onReadResponse(readResponseMessage);
        break;
    }
    case Message::DELETE_RESPONSE:
    {
        auto deleteResponseMessage = dynamic_cast<DeleteResponseMessage*>(message.get());
        onDeleteResponse(deleteResponseMessage);
        break;
    }
    }

    throw utils::NotImplementedException();
}

} // namespace key_value_store
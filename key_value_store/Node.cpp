#include "Node.h"
#include "CreateRequestMessage.h"
#include "CreateResponseMessage.h"
#include "DeleteRequestMessage.h"
#include "DeleteResponseMessage.h"
#include "ReadRequestMessage.h"
#include "ReadResponseMessage.h"
#include "RepairRequestMessage.h"
#include "UpdateRequestMessage.h"
#include "UpdateResponseMessage.h"
#include "key_value_store/Exceptions.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"
#include <array>
#include <exception>
#include <functional>
#include <future>
#include <thread>

namespace key_value_store
{
enum ResponseCode
{
    OK = 200,
    NOT_FOUND = 404,
    CONFLICT = 409,
};

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
    , asyncQueueCallback([this](std::unique_ptr<utils::Message> message) { asyncQueue.push(utils::Utils::downcast<key_value_store::Message, utils::Message>(std::move(message))); })
    , sentMessages()
    , runnable([this]() { run(); })
    , threadPolicy(std::move(threadPolicy))
    , tokens()
{
}

void Node::start()
{
    logger->log(address, "Starting node");
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<CreateRequestMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<UpdateRequestMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<ReadRequestMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<DeleteRequestMessage>(), asyncQueueCallback));

    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<CreateResponseMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<UpdateResponseMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<ReadResponseMessage>(), asyncQueueCallback));
    tokens.push_back(messageDispatcher->listen(utils::Message::getTypeName<DeleteResponseMessage>(), asyncQueueCallback));

    asyncQueue.start();
    membershipProtocol->start();

    runnable.start();
}

void Node::stop()
{
    runnable.stop();
    membershipProtocol->stop();
    asyncQueue.stop();

    for (auto token : tokens)
    {
        messageDispatcher->stopListening(token);
    }

    logger->log(address, "Stopped node");
}

void Node::create(const std::string& key, const std::string& value)
{
    auto timestamp = utils::Time::getTimestamp();
    processClientRequest<bool>(key, [this, key, value, timestamp]() -> bool { return createLocally(key, value, timestamp); }, [this, key, value, timestamp](const network::Address& node) -> bool { return sendCreateMessage(node, key, value, timestamp); });
}

bool Node::createLocally(const std::string& key, const std::string& value, unsigned long timestamp)
{
    storage->insert(key, Record(value, timestamp));
    return true;
}

bool Node::sendCreateMessage(const network::Address& target, const std::string& key, const std::string& value, unsigned long timestamp)
{
    auto message = std::make_unique<CreateRequestMessage>(address, target, key, value, timestamp);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto createResponse = static_cast<CreateResponseMessage*>(response.get());
    return createResponse->getResponseCode() == OK;
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
        throw std::logic_error("request timed out");
    }

    auto readResponse = static_cast<ReadResponseMessage*>(response.get());
    if (response->getResponseCode() != OK)
    {
        throw std::logic_error("TBD");
    }

    return Record(readResponse->getValue(), readResponse->getTimestamp());
}

void Node::update(const std::string& key, const std::string& value)
{
    auto timestamp = utils::Time::getTimestamp();
    processClientRequest<bool>(key, [this, key, value, timestamp]() -> bool { return updateLocally(key, value, timestamp); }, [this, key, value, timestamp](const network::Address& node) -> bool { return sendUpdateMessage(node, key, value, timestamp); });
}

bool Node::updateLocally(const std::string& key, const std::string& value, unsigned long timestamp)
{
    storage->update(key, Record(value, timestamp));
    return true;
}

bool Node::sendUpdateMessage(const network::Address& target, const std::string& key, const std::string& value, unsigned long timestamp)
{
    auto message = std::make_unique<UpdateRequestMessage>(address, target, key, value, timestamp);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto updateResponse = static_cast<UpdateResponseMessage*>(response.get());
    return updateResponse->getResponseCode() == OK;
}

void Node::remove(const std::string& key)
{
    auto timestamp = utils::Time::getTimestamp();
    processClientRequest<bool>(key, [this, key, timestamp]() -> bool { return removeLocally(key, timestamp); }, [this, key, timestamp](const network::Address& node) -> bool { return sendRemoveMessage(node, key, timestamp); });
}

bool Node::removeLocally(const std::string& key, unsigned long timestamp)
{
    storage->remove(key, timestamp);
    return true;
}

bool Node::sendRemoveMessage(const network::Address& target, const std::string& key, unsigned long timestamp)
{
    auto message = std::make_unique<DeleteRequestMessage>(address, target, key, timestamp);
    auto response = sendMessageAndWait(std::move(message), 2s);
    if (!response)
    {
        return false;
    }

    auto deleteResponse = static_cast<DeleteResponseMessage*>(response.get());
    return deleteResponse->getResponseCode() == OK;
}

bool Node::sendRepairMessage(const network::Address& target, const std::string& key, const std::string& value, unsigned long timestamp)
{
    auto message = std::make_unique<RepairRequestMessage>(address, target, key, value, timestamp);
    logger->log("sending ", message->getMsgTypeStr(), "from ", address, "to ", target, message->getId(), message->getKey());
    messageDispatcher->sendMessage(std::move(message), target);
    logger->log("sent from ", address, "to ", target);

    // todo: wait for response?
    return true;
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
    Record record;
    try
    {
        // TODO: make sure it belongs to this node
        auto timestamp = message->getTimestamp();
        storage->insert(key, Record(message->getValue(), timestamp));
    }
    catch (ConflictDetected)
    {
        messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), CONFLICT), message->getSourceAddress());
        return;
    }

    messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
}

void Node::onUpdateRequest(std::unique_ptr<UpdateRequestMessage> message)
{
    auto key = message->getKey();
    try
    {
        // TODO: make sure it belongs to this node
        auto timestamp = message->getTimestamp();
        storage->update(key, Record(message->getValue(), timestamp));
    }
    catch (NotFoundException)
    {
        messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND), message->getSourceAddress());
        return;
    }

    messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
}

void Node::onReadRequest(std::unique_ptr<ReadRequestMessage> message)
{
    auto key = message->getKey();
    Record record;
    try
    {
        // TODO: make sure it belongs to this node
        record = storage->get(key);
    }
    catch (key_value_store::NotFoundException)
    {
        messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND, "", 0), message->getSourceAddress());
        return;
    }

    messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), OK, record.value, record.timestamp), message->getSourceAddress());
}

void Node::onDeleteRequest(std::unique_ptr<DeleteRequestMessage> message)
{
    auto key = message->getKey();
    try
    {
        // TODO: make sure it belongs to this node
        auto timestamp = message->getTimestamp();
        storage->remove(key, timestamp);
    }
    catch (key_value_store::NotFoundException)
    {
        messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND), message->getSourceAddress());
        return;
    }

    messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
}

// void Node::onCreateRequest(std::unique_ptr<CreateRequestMessage> message)
// {
//     auto key = message->getKey();
//     Record record;
//     bool found = getFromStorage(key, record);
//     auto timestamp = message->getTimestamp();
//     if (found && (!record.removed || record.timestamp >= timestamp))
//     {
//         messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), CONFLICT), message->getSourceAddress());
//         return;
//     }

//     // TODO: make sure it belongs to this node
//     storage->insert(message->getKey(), Record(message->getValue(), message->getTimestamp()));
//     messageDispatcher->sendMessage(std::make_unique<CreateResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
// }

// void Node::onUpdateRequest(std::unique_ptr<UpdateRequestMessage> message)
// {
//     auto key = message->getKey();
//     Record record;
//     bool found = getFromStorage(key, record);
//     auto timestamp = message->getTimestamp();
//     if (!found || record.removed && record.timestamp < timestamp)
//     {
//         messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND), message->getSourceAddress());
//         return;
//     }

//     if (record.timestamp >= timestamp)
//     {
//         messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), CONFLICT), message->getSourceAddress());
//         return;
//     }

//     // TODO: make sure it belongs to this node
//     storage->update(key, Record(message->getValue(), timestamp));
//     messageDispatcher->sendMessage(std::make_unique<UpdateResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
// }

// void Node::onReadRequest(std::unique_ptr<ReadRequestMessage> message)
// {
//     auto key = message->getKey();

//     // TODO: make sure it belongs to this node
//     Record record;
//     bool found = getFromStorage(key, record);
//     if (!found || record.removed)
//     {
//         messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND, "", 0), message->getSourceAddress());
//         return;
//     }

//     messageDispatcher->sendMessage(std::make_unique<ReadResponseMessage>(address, message->getSourceAddress(), message->getId(), OK, record.value, record.timestamp), message->getSourceAddress());
// }

// void Node::onDeleteRequest(std::unique_ptr<DeleteRequestMessage> message)
// {
//     auto key = message->getKey();
//     Record record;
//     bool found = getFromStorage(key, record);
//     if (!found || record.removed)
//     {
//         messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), NOT_FOUND, "", 0), message->getSourceAddress());
//         return
//     }

//     auto timestamp = message->getTimestamp();
//     if (record.timestamp >= timestamp)
//     {
//         messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), CONFLICT), message->getSourceAddress());
//         return;
//     }

//     // TODO: make sure it belongs to this node
//     storage->remove(message->getKey());
//     // TODO: handle exceptions

//     messageDispatcher->sendMessage(std::make_unique<DeleteResponseMessage>(address, message->getSourceAddress(), message->getId(), OK), message->getSourceAddress());
// }

void Node::onRepairRequest(std::unique_ptr<RepairRequestMessage> message)
{
    auto key = message->getKey();
    auto value = message->getValue();
    auto timestamp = message->getTimestamp();
    logger->log("Received repair request from ", message->getSourceAddress(), "for ", key, "value: ", value);

    Record record;
    try
    {
        record = storage->get(message->getKey());
    }
    catch (key_value_store::NotFoundException)
    {
        logger->log("Couldn't find ", key, "in local storage, storing value", value, "timestamp", message->getTimestamp());
        storage->insert(key, Record(value, timestamp));
        return;
    }

    if (record.timestamp == timestamp && record.value != value)
    {
        logger->log("Conflict found, ", key, " has a value ", record.value, "trying to set it to", value);
        return;
    }
    else if (record.timestamp > timestamp)
    {
        logger->log(key, " has a newer value ", record.value, "vs", value);
        return;
    }

    // TODO: make sure it belongs to this node
    storage->update(key, Record(value, timestamp));
    // TODO: handle exceptions
}

void Node::onResponse(std::unique_ptr<ResponseMessage> message)
{
    auto messageId = message->getOriginalMessageId();
    std::unique_lock<std::mutex> lock(mutex);
    assert(sentMessages.find(messageId) != sentMessages.end());

    sentMessages[messageId]->responseMessage.reset(message.release());
}

void Node::processMessage(std::unique_ptr<Message> message)
{
    logger->log("Received message ", message->getMsgTypeStr(), " from ", message->getSourceAddress(), message->getId());

    switch (message->getMessageType())
    {
    case Message::CREATE_REQUEST:
    {
        std::unique_ptr<CreateRequestMessage> createRequestMessage(dynamic_cast<CreateRequestMessage*>(message.release()));
        onCreateRequest(std::move(createRequestMessage));
        return;
    }
    case Message::UPDATE_REQUEST:
    {
        std::unique_ptr<UpdateRequestMessage> updateRequestMessage(dynamic_cast<UpdateRequestMessage*>(message.release()));
        onUpdateRequest(std::move(updateRequestMessage));
        return;
    }
    case Message::READ_REQUEST:
    {
        std::unique_ptr<ReadRequestMessage> readRequestMessage(dynamic_cast<ReadRequestMessage*>(message.release()));
        onReadRequest(std::move(readRequestMessage));
        return;
    }
    case Message::DELETE_REQUEST:
    {
        std::unique_ptr<DeleteRequestMessage> deleteRequestMessage(dynamic_cast<DeleteRequestMessage*>(message.release()));
        onDeleteRequest(std::move(deleteRequestMessage));
        return;
    }

    case Message::REPAIR_REQUEST:
    {
        std::unique_ptr<RepairRequestMessage> repairRequestMessage(dynamic_cast<RepairRequestMessage*>(message.release()));
        onRepairRequest(std::move(repairRequestMessage));
        return;
    }

    case Message::CREATE_RESPONSE:
    case Message::UPDATE_RESPONSE:
    case Message::READ_RESPONSE:
    case Message::DELETE_RESPONSE:
    {
        std::unique_ptr<ResponseMessage> responseMessage(dynamic_cast<ResponseMessage*>(message.release()));
        onResponse(std::move(responseMessage));
        return;
    }
    }

    throw utils::NotImplementedException();
}

void Node::runStabilizationProtocol()
{
    auto records = storage->getRecords();
    for (auto it = records.begin(); it != records.end(); ++it)
    {
        auto timestamp = utils::Time::getTimestamp();
        auto nodes = getTargetNodes(it->first);
        auto curNodeIt = std::find_if(nodes.begin(), nodes.end(), [this](const network::Address& addr) -> bool { return addr == address; });
        if (curNodeIt == nodes.end())
        {
            for (auto nodeIt = nodes.begin(); nodeIt != nodes.end(); ++nodeIt)
            {
                sendRepairMessage(*nodeIt, it->first, it->second.value, timestamp);
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

            if (nodes.size() > i && isNodeAlive(nodes[i]))
            {
                sendRepairMessage(nodes[i], it->first, it->second.value, timestamp);
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
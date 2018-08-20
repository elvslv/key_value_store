#include "RequestMessage.h"
#include "CreateRequestMessage.h"
#include "DeleteRequestMessage.h"
#include "ReadRequestMessage.h"
#include "UpdateRequestMessage.h"
#include "utils/Exceptions.h"
#include "utils/TimeUtils.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id, unsigned long timestamp)
    : Message(messageType, sourceAddress, destinationAddress, id)
    , key(key)
    , timestamp(timestamp)
{
}

RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, unsigned long timestamp)
    : Message(messageType, sourceAddress, destinationAddress)
    , key(key)
    , timestamp(timestamp)
{
}

RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : Message(messageType, sourceAddress, destinationAddress)
    , key(key)
    , timestamp(utils::Time::getTimestamp())
{
}

std::string RequestMessage::getKey() const
{
    return key;
}

unsigned long RequestMessage::getTimestamp() const
{
    return timestamp;
}

gen::Message RequestMessage::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();
    auto kvStoreMessage = getKeyValueStoreMessage(message);

    auto requestMessage = std::make_unique<gen::key_value_store::RequestMessage>();
    requestMessage->set_messagetype(getProtobufMessageType());
    requestMessage->set_key(key);
    requestMessage->set_timestamp(timestamp);

    kvStoreMessage->set_allocated_requestmessage(requestMessage.release());
    return message;
}

gen::key_value_store::RequestMessageType RequestMessage::getProtobufMessageType() const
{
    switch (getMessageType())
    {
    case CREATE_REQUEST:
        return gen::key_value_store::CREATE_REQUEST;

    case UPDATE_REQUEST:
        return gen::key_value_store::UPDATE_REQUEST;

    case READ_REQUEST:
        return gen::key_value_store::READ_REQUEST;

    case DELETE_REQUEST:
        return gen::key_value_store::DELETE_REQUEST;

    case REPAIR_REQUEST:
        return gen::key_value_store::REPAIR_REQUEST;
    }

    throw utils::NotImplementedException();
}

gen::key_value_store::RequestMessage* RequestMessage::getRequestMessage(gen::Message& message)
{
    auto kvStoreMessage = getKeyValueStoreMessage(message);
    if (kvStoreMessage->has_requestmessage())
    {
        return kvStoreMessage->mutable_requestmessage();
    }

    throw std::logic_error("Request message is not set");
}
}
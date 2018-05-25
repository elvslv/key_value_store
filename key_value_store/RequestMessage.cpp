#include "RequestMessage.h"
#include "CreateRequestMessage.h"
#include "DeleteRequestMessage.h"
#include "ReadRequestMessage.h"
#include "UpdateRequestMessage.h"
#include "utils/Exceptions.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : Message(messageType, sourceAddress, destinationAddress, id)
    , key(key)
{
}

RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : Message(messageType, sourceAddress, destinationAddress)
    , key(key)
{
}

std::string RequestMessage::getKey() const
{
    return key;
}

gen::key_value_store::Message RequestMessage::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();
    auto requestMessage = std::make_unique<gen::key_value_store::RequestMessage>();
    requestMessage->set_messagetype(getProtobufMessageType());
    requestMessage->set_key(key);

    message.set_allocated_requestmessage(requestMessage.release());
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
    }

    throw utils::NotImplementedException();
}

std::string RequestMessage::getMsgTypeStr() const
{
    switch (getMessageType())
    {
    case CREATE_REQUEST:
        return "CREATE_REQUEST";

    case UPDATE_REQUEST:
        return "UPDATE_REQUEST";

    case READ_REQUEST:
        return "READ_REQUEST";

    case DELETE_REQUEST:
        return "DELETE_REQUEST";
    }

    throw utils::NotImplementedException();
}

gen::key_value_store::RequestMessage* RequestMessage::getRequestMessage(gen::key_value_store::Message& message)
{
    if (message.has_requestmessage())
    {
        return message.mutable_requestmessage();
    }

    throw std::logic_error("Request message is not set");
}
}
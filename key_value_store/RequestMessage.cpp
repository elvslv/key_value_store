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
    : Message(sourceAddress, destinationAddress, key, id)
    , messageType(messageType)
{
}

RequestMessage::RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : Message(sourceAddress, destinationAddress, key)
    , messageType(messageType)
{
}

RequestMessage::MsgTypes RequestMessage::getMessageType() const
{
    return messageType;
}

std::string RequestMessage::getKey() const
{
    return key;
}

std::string RequestMessage::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr(messageType) << " request from " << sourceAddress.toString() << " to " << destinationAddress.toString() << "id " << id << std::endl;
    return ss.str();
}

network::Message RequestMessage::serialize() const
{
    gen::Message message = serializeToProtobuf();

    unsigned int size = message.ByteSize();
    char* data = new char[size];
    message.SerializeToArray(data, size);

    return network::Message(data, size);
}

gen::Message RequestMessage::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();
    auto requestMessage = std::make_unique<gen::RequestMessage>();
    requestMessage->set_messagetype(getProtobufMessageType());
    requestMessage->set_key(key);

    message.set_allocated_requestmessage(requestMessage.release());
    return message;
}

gen::RequestMessageType RequestMessage::getProtobufMessageType() const
{
    switch (messageType)
    {
    case CREATE:
        return gen::CREATE_REQUEST;

    case UPDATE:
        return gen::UPDATE_REQUEST;

    case READ:
        return gen::READ_REQUEST;

    case DELETE:
        return gen::DELETE_REQUEST;
    }

    throw utils::NotImplementedException();
}

std::string RequestMessage::getMsgTypeStr(MsgTypes msgType)
{
    switch (msgType)
    {
    case CREATE:
        return "CREATE";

    case UPDATE:
        return "UPDATE";

    case READ:
        return "READ";

    case DELETE:
        return "DELETE";
    }

    throw utils::NotImplementedException();
}

gen::RequestMessage RequestMessage::getRequestMessage(const gen::Message& message)
{
    if (message.has_requestmessage())
    {
        return message.requestmessage();
    }

    throw std::logic_error("Request message is not set");
}
}
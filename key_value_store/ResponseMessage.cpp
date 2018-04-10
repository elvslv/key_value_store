#include "ResponseMessage.h"
#include "utils/Exceptions.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : Message(sourceAddress, destinationAddress, key, id)
    , messageType(messageType)
{
}

ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : Message(sourceAddress, destinationAddress, key)
    , messageType(messageType)
{
}

ResponseMessage::MsgTypes ResponseMessage::getMessageType() const
{
    return messageType;
}

gen::ResponseMessageType ResponseMessage::getProtobufMessageType() const
{
    switch (messageType)
    {
    case CREATE:
        return gen::CREATE_RESPONSE;

    case UPDATE:
        return gen::UPDATE_RESPONSE;

    case READ:
        return gen::READ_RESPONSE;

    case DELETE:
        return gen::DELETE_RESPONSE;
    }

    throw utils::NotImplementedException();
}

std::string ResponseMessage::getMsgTypeStr() const
{
    switch (messageType)
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

gen::ResponseMessage ResponseMessage::getResponseMessage(const gen::Message& message)
{
    if (message.has_responsemessage())
    {
        return message.responsemessage();
    }

    throw std::logic_error("Request message is not set");
}
}
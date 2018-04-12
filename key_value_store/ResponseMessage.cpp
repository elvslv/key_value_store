#include "ResponseMessage.h"
#include "utils/Exceptions.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id)
    : Message(sourceAddress, destinationAddress, id)
    , messageType(messageType)
    , originalMessageId(originalMessageId)
    , responseCode(responseCode)
{
}

ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode)
    : Message(sourceAddress, destinationAddress)
    , messageType(messageType)
    , originalMessageId(originalMessageId)
    , responseCode(responseCode)
{
}

ResponseMessage::MsgTypes ResponseMessage::getMessageType() const
{
    return messageType;
}

std::string ResponseMessage::getOriginalMessageId() const
{
    return originalMessageId;
}

unsigned int ResponseMessage::getResponseCode() const
{
    return responseCode;
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
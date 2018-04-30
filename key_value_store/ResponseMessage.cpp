#include "ResponseMessage.h"
#include "utils/Exceptions.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id)
    : Message(messageType, sourceAddress, destinationAddress, id)
    , originalMessageId(originalMessageId)
    , responseCode(responseCode)
{
}

ResponseMessage::ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode)
    : Message(messageType, sourceAddress, destinationAddress)
    , originalMessageId(originalMessageId)
    , responseCode(responseCode)
{
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
    case CREATE_RESPONSE:
        return gen::CREATE_RESPONSE;

    case UPDATE_RESPONSE:
        return gen::UPDATE_RESPONSE;

    case READ_RESPONSE:
        return gen::READ_RESPONSE;

    case DELETE_RESPONSE:
        return gen::DELETE_RESPONSE;
    }

    throw utils::NotImplementedException();
}

std::string ResponseMessage::getMsgTypeStr() const
{
    switch (messageType)
    {
    case CREATE_RESPONSE:
        return "CREATE_RESPONSE";

    case UPDATE_RESPONSE:
        return "UPDATE_RESPONSE";

    case READ_RESPONSE:
        return "READ_RESPONSE";

    case DELETE_RESPONSE:
        return "DELETE_RESPONSE";
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
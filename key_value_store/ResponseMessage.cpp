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

gen::key_value_store::ResponseMessageType ResponseMessage::getProtobufMessageType() const
{
    switch (getMessageType())
    {
    case CREATE_RESPONSE:
        return gen::key_value_store::CREATE_RESPONSE;

    case UPDATE_RESPONSE:
        return gen::key_value_store::UPDATE_RESPONSE;

    case READ_RESPONSE:
        return gen::key_value_store::READ_RESPONSE;

    case DELETE_RESPONSE:
        return gen::key_value_store::DELETE_RESPONSE;
    }

    throw utils::NotImplementedException();
}

gen::key_value_store::ResponseMessage* ResponseMessage::getResponseMessage(gen::Message& message)
{
    auto kvStoreMessage = getKeyValueStoreMessage(message);
    if (kvStoreMessage->has_responsemessage())
    {
        return kvStoreMessage->mutable_responsemessage();
    }

    throw std::logic_error("Response message is not set");
}

gen::Message ResponseMessage::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();
    auto kvStoreMessage = getKeyValueStoreMessage(message);

    auto responseMessage = std::make_unique<gen::key_value_store::ResponseMessage>();
    responseMessage->set_messagetype(getProtobufMessageType());
    responseMessage->set_originalmessageid(originalMessageId);
    responseMessage->set_responsecode(responseCode);

    kvStoreMessage->set_allocated_responsemessage(responseMessage.release());
    return message;
}
}
#include "ReadResponseMessage.h"

namespace key_value_store
{
ReadResponseMessage::ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, const std::string& id)
    : ResponseMessage(READ_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode, id)
    , value(value)
{
}

ReadResponseMessage::ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value)
    : ResponseMessage(READ_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode)
    , value(value)
{
}

std::string ReadResponseMessage::getValue() const
{
    return value;
}

gen::Message ReadResponseMessage::serializeToProtobuf() const
{
    auto message = ResponseMessage::serializeToProtobuf();

    auto readResponseFields = std::make_unique<gen::key_value_store::ReadResponseFields>();
    auto responseMessage = getResponseMessage(message);
    readResponseFields->set_value(value);
    responseMessage->set_allocated_readfields(readResponseFields.release());

    return message;
}
}
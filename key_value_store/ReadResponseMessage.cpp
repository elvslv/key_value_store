#include "ReadResponseMessage.h"

namespace key_value_store
{
ReadResponseMessage::ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, unsigned long timestamp, const std::string& id)
    : ResponseMessage(READ_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode, id)
    , value(value)
    , timestamp(timestamp)
{
}

ReadResponseMessage::ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, unsigned long timestamp)
    : ResponseMessage(READ_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode)
    , value(value)
    , timestamp(timestamp)
{
}

std::string ReadResponseMessage::getValue() const
{
    return value;
}

unsigned long ReadResponseMessage::getTimestamp() const
{
    return timestamp;
}

gen::Message ReadResponseMessage::serializeToProtobuf() const
{
    auto message = ResponseMessage::serializeToProtobuf();

    auto readResponseFields = std::make_unique<gen::key_value_store::ReadResponseFields>();
    auto responseMessage = getResponseMessage(message);
    readResponseFields->set_value(value);
    readResponseFields->set_timestamp(timestamp);

    responseMessage->set_allocated_readfields(readResponseFields.release());

    return message;
}
}
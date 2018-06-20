#include "ReadRequestMessage.h"

namespace key_value_store
{
ReadRequestMessage::ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : RequestMessage(READ_REQUEST, sourceAddress, destinationAddress, key, id)
{
}

ReadRequestMessage::ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : RequestMessage(READ_REQUEST, sourceAddress, destinationAddress, key)
{
}

gen::Message ReadRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto requestMessage = getRequestMessage(message);
    auto readRequestFields = std::make_unique<gen::key_value_store::ReadRequestFields>();
    requestMessage->set_allocated_readfields(readRequestFields.release());

    return message;
}
}
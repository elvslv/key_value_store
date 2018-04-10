#include "ReadRequestMessage.h"

namespace key_value_store
{
ReadRequestMessage::ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : RequestMessage(READ, sourceAddress, destinationAddress, key, id)
{
}

ReadRequestMessage::ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : RequestMessage(READ, sourceAddress, destinationAddress, key)
{
}

gen::Message ReadRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto requestMessage = getRequestMessage(message);
    auto readRequestFields = std::make_unique<gen::ReadRequestFields>();
    requestMessage.set_allocated_readfields(readRequestFields.release());
}
}
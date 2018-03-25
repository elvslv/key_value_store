#include "DeleteRequestMessage.h"

namespace key_value_store
{
DeleteRequestMessage::DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : RequestMessage(DELETE, sourceAddress, destinationAddress, key, id)
{
}

DeleteRequestMessage::DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : RequestMessage(DELETE, sourceAddress, destinationAddress, key)
{
}

gen::RequestMessage DeleteRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto deleteRequestFields = std::make_unique<gen::DeleteRequestFields>();
    message.set_allocated_deletefields(deleteRequestFields.release());
}
}
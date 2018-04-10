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

gen::Message DeleteRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto requestMessage = getRequestMessage(message);
    auto deleteRequestFields = std::make_unique<gen::DeleteRequestFields>();
    requestMessage.set_allocated_deletefields(deleteRequestFields.release());
}
}
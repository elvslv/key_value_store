#include "DeleteRequestMessage.h"

namespace key_value_store
{
DeleteRequestMessage::DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id, unsigned long timestamp)
    : RequestMessage(DELETE_REQUEST, sourceAddress, destinationAddress, key, id, timestamp)
{
}

DeleteRequestMessage::DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, unsigned long timestamp)
    : RequestMessage(DELETE_REQUEST, sourceAddress, destinationAddress, key, timestamp)
{
}

DeleteRequestMessage::DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : RequestMessage(DELETE_REQUEST, sourceAddress, destinationAddress, key)
{
}

gen::Message DeleteRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto requestMessage = getRequestMessage(message);
    auto deleteRequestFields = std::make_unique<gen::key_value_store::DeleteRequestFields>();
    requestMessage->set_allocated_deletefields(deleteRequestFields.release());

    return message;
}
}
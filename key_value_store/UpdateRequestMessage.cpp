#include "UpdateRequestMessage.h"

namespace key_value_store
{
UpdateRequestMessage::UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id)
    : RequestMessage(UPDATE, sourceAddress, destinationAddress, key, id)
    , value(value)
{
}

UpdateRequestMessage::UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value)
    : RequestMessage(UPDATE, sourceAddress, destinationAddress, key)
    , value(value)
{
}

gen::Message UpdateRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto updateRequestFields = std::make_unique<gen::UpdateRequestFields>();
    auto val = std::make_unique<std::string>(value);
    updateRequestFields->set_allocated_value(val.release());

    auto requestMessage = getRequestMessage(message);
    requestMessage.set_allocated_updatefields(updateRequestFields.release());

    return message;
}
}
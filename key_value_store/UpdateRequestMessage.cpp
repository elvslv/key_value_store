#include "UpdateRequestMessage.h"

namespace key_value_store
{
UpdateRequestMessage::UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id)
    : RequestMessage(UPDATE_REQUEST, sourceAddress, destinationAddress, key, id)
    , value(value)
{
}

UpdateRequestMessage::UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value)
    : RequestMessage(UPDATE_REQUEST, sourceAddress, destinationAddress, key)
    , value(value)
{
}

std::string UpdateRequestMessage::getValue() const
{
    return value;
}

gen::Message UpdateRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto updateRequestFields = std::make_unique<gen::key_value_store::UpdateRequestFields>();
    auto val = std::make_unique<std::string>(value);
    updateRequestFields->set_allocated_value(val.release());

    auto requestMessage = getRequestMessage(message);
    requestMessage->set_allocated_updatefields(updateRequestFields.release());

    return message;
}
}
#include "CreateRequestMessage.h"

namespace key_value_store
{
CreateRequestMessage::CreateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id)
    : RequestMessage(CREATE, sourceAddress, destinationAddress, key, id)
    , value(value)
{
}

CreateRequestMessage::CreateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value)
    : RequestMessage(CREATE, sourceAddress, destinationAddress, key)
    , value(value)
{
}

gen::Message CreateRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto createRequestFields = std::make_unique<gen::CreateRequestFields>();
    auto val = std::make_unique<std::string>(value);
    createRequestFields->set_allocated_value(val.release());

    auto requestMessage = getRequestMessage(message);
    requestMessage.set_allocated_createfields(createRequestFields.release());

    return message;
}
}
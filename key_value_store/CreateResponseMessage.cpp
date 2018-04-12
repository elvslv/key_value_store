#include "CreateResponseMessage.h"

namespace key_value_store
{
CreateResponseMessage::CreateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id)
    : ResponseMessage(ResponseMessage::CREATE, sourceAddress, destinationAddress, originalMessageId, responseCode, id)
{
}

CreateResponseMessage::CreateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode)
    : ResponseMessage(ResponseMessage::CREATE, sourceAddress, destinationAddress, originalMessageId, responseCode)
{
}

gen::Message CreateResponseMessage::serializeToProtobuf() const
{
    auto message = ResponseMessage::serializeToProtobuf();

    auto createResponseFields = std::make_unique<gen::CreateResponseFields>();
    auto responseMessage = getResponseMessage(message);
    responseMessage.set_allocated_createfields(createResponseFields.release());

    return message;
}
}
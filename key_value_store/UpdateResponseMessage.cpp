#include "UpdateResponseMessage.h"

namespace key_value_store
{
UpdateResponseMessage::UpdateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id)
    : ResponseMessage(UPDATE_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode, id)
{
}

UpdateResponseMessage::UpdateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode)
    : ResponseMessage(UPDATE_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode)
{
}

gen::Message UpdateResponseMessage::serializeToProtobuf() const
{
    auto message = ResponseMessage::serializeToProtobuf();

    auto updateResponseFields = std::make_unique<gen::key_value_store::UpdateResponseFields>();
    auto responseMessage = getResponseMessage(message);
    responseMessage->set_allocated_updatefields(updateResponseFields.release());

    return message;
}
}
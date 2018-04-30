#include "DeleteResponseMessage.h"

namespace key_value_store
{
DeleteResponseMessage::DeleteResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id)
    : ResponseMessage(DELETE_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode, id)
{
}

DeleteResponseMessage::DeleteResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode)
    : ResponseMessage(DELETE_RESPONSE, sourceAddress, destinationAddress, originalMessageId, responseCode)
{
}

gen::Message DeleteResponseMessage::serializeToProtobuf() const
{
    auto message = ResponseMessage::serializeToProtobuf();

    auto deleteResponseFields = std::make_unique<gen::DeleteResponseFields>();
    auto responseMessage = getResponseMessage(message);
    responseMessage.set_allocated_deletefields(deleteResponseFields.release());

    return message;
}
}
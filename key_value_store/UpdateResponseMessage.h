#pragma once

#include "ResponseMessage.h"

namespace key_value_store
{
class UpdateResponseMessage : public ResponseMessage
{
public:
    UpdateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id);
    UpdateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode);

protected:
    virtual gen::Message serializeToProtobuf() const;
};
}
#pragma once

#include "ResponseMessage.h"

namespace key_value_store
{
class CreateResponseMessage : public ResponseMessage
{
public:
    CreateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id);
    CreateResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode);

protected:
    virtual gen::Message serializeToProtobuf() const;
};
}
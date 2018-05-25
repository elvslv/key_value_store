#pragma once

#include "ResponseMessage.h"

namespace key_value_store
{
class ReadResponseMessage : public ResponseMessage
{
public:
    ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, const std::string& id);
    ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value);

    std::string getValue() const;

protected:
    virtual gen::key_value_store::Message serializeToProtobuf() const;

private:
    std::string value;
};
}
#pragma once

#include "ResponseMessage.h"

namespace key_value_store
{
class ReadResponseMessage : public ResponseMessage
{
public:
    ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, unsigned long timestamp, const std::string& id);
    ReadResponseMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& value, unsigned long timestamp);

    std::string getValue() const;
    unsigned long getTimestamp() const;

protected:
    virtual gen::Message serializeToProtobuf() const;

private:
    std::string value;
    unsigned long timestamp;
};
}
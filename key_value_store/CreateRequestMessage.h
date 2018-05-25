#pragma once
#include "RequestMessage.h"

namespace key_value_store
{
class CreateRequestMessage : public RequestMessage
{
public:
    CreateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id);
    CreateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value);

    std::string getValue() const;

protected:
    virtual gen::key_value_store::Message serializeToProtobuf() const;

private:
    std::string value;
};
}
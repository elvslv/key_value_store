#pragma once
#include "RequestMessage.h"

namespace key_value_store
{
class UpdateRequestMessage : public RequestMessage
{
public:
    UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id);
    UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value);

    std::string getValue() const;

protected:
    virtual gen::key_value_store::Message serializeToProtobuf() const;

private:
    std::string value;
};
}
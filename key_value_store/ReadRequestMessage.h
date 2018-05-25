#pragma once
#include "RequestMessage.h"

namespace key_value_store
{
class ReadRequestMessage : public RequestMessage
{
public:
    ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    ReadRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

protected:
    virtual gen::key_value_store::Message serializeToProtobuf() const;
};
}
#pragma once
#include "RequestMessage.h"

namespace key_value_store
{
class DeleteRequestMessage : public RequestMessage
{
public:
    DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id, unsigned long timestamp);
    DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, unsigned long timestamp);
    DeleteRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

protected:
    virtual gen::Message serializeToProtobuf() const;
};
}
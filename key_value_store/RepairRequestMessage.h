#pragma once
#include "RequestMessage.h"

namespace key_value_store
{
class RepairRequestMessage : public RequestMessage
{
public:
    RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id, unsigned long timestamp);
    RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, unsigned long timestamp);
    RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value);

    std::string getValue() const;

protected:
    virtual gen::Message serializeToProtobuf() const;

private:
    std::string value;
};
}
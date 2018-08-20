#include "RepairRequestMessage.h"

namespace key_value_store
{
RepairRequestMessage::RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id, unsigned long timestamp)
    : RequestMessage(REPAIR_REQUEST, sourceAddress, destinationAddress, key, id, timestamp)
    , value(value)
{
}

RepairRequestMessage::RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, unsigned long timestamp)
    : RequestMessage(REPAIR_REQUEST, sourceAddress, destinationAddress, key, timestamp)
    , value(value)
{
}

RepairRequestMessage::RepairRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value)
    : RequestMessage(REPAIR_REQUEST, sourceAddress, destinationAddress, key)
    , value(value)
{
}

std::string RepairRequestMessage::getValue() const
{
    return value;
}

gen::Message RepairRequestMessage::serializeToProtobuf() const
{
    auto message = RequestMessage::serializeToProtobuf();

    auto repairRequestFields = std::make_unique<gen::key_value_store::RepairRequestFields>();
    auto val = std::make_unique<std::string>(value);
    repairRequestFields->set_allocated_value(val.release());

    auto requestMessage = getRequestMessage(message);
    requestMessage->set_allocated_repairfields(repairRequestFields.release());

    return message;
}
}
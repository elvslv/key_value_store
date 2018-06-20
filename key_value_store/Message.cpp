#include "Message.h"
#include "CreateRequestMessage.h"
#include "CreateResponseMessage.h"
#include "DeleteRequestMessage.h"
#include "DeleteResponseMessage.h"
#include "ReadRequestMessage.h"
#include "ReadResponseMessage.h"
#include "RequestMessage.h"
#include "UpdateRequestMessage.h"
#include "UpdateResponseMessage.h"
#include "utils/Exceptions.h"

#include <cassert>
#include <sstream>

namespace key_value_store
{
Message::Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id)
    : utils::Message(sourceAddress, destinationAddress, id)
    , messageType(messageType)
{
}

Message::Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress)
    : utils::Message(sourceAddress, destinationAddress)
    , messageType(messageType)
{
}

std::string Message::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr() << " request from " << sourceAddress.toString() << " to " << destinationAddress.toString() << "id " << id << std::endl;
    return ss.str();
}

Message::MsgTypes Message::getMessageType() const
{
    return messageType;
}

gen::key_value_store::Message* Message::getKeyValueStoreMessage(gen::Message& message)
{
    if (message.has_keyvaluestoremessage())
    {
        return message.mutable_keyvaluestoremessage();
    }

    throw std::logic_error("KV store message is not set");
}

gen::Message Message::serializeToProtobuf() const
{
    auto message = utils::Message::serializeToProtobuf();
    auto kvStoreMessage = std::make_unique<gen::key_value_store::Message>();
    message.set_allocated_keyvaluestoremessage(kvStoreMessage.release());

    return message;
}
}
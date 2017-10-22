#include "AckMessage.h"

namespace membership_protocol
{
    AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::string& pingMessageId) : 
        Message(ACK, from, to),
        pingMessageId(pingMessageId)
    {
    }

    AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::string& pingMessageId) :
        Message(ACK, from, to, id),
        pingMessageId(pingMessageId)
    {
    }

    gen::MessageType AckMessage::getProtobufMessageType() const
    {
        return gen::ACK;
    }

    gen::Message AckMessage::serializeToProtobuf() const
    {
        auto message = Message::serializeToProtobuf();

        auto ackFields = std::make_unique<gen::AckFields>();
        ackFields->set_originalid(pingMessageId);
        message.set_allocated_ackfields(ackFields.release());

        return message;
    }

}

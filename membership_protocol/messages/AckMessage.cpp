#include "AckMessage.h"

namespace membership_protocol
{
    AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& pingMessageId) : 
        MessageWithGossipsBase(ACK, from, to, gossips),
        pingMessageId(pingMessageId)
    {
    }

    AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id, const std::string& pingMessageId) :
        MessageWithGossipsBase(ACK, from, to, id, gossips),
        pingMessageId(pingMessageId)
    {
    }

    gen::MessageType AckMessage::getProtobufMessageType() const
    {
        return gen::ACK;
    }

    gen::Message AckMessage::serializeToProtobuf() const
    {
        auto message = MessageWithGossipsBase::serializeToProtobuf();

        auto ackFields = std::make_unique<gen::AckFields>();
        ackFields->set_originalid(pingMessageId);
        message.set_allocated_ackfields(ackFields.release());

        return message;
    }

}

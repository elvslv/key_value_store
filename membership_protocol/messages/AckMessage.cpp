#include "AckMessage.h"

namespace membership_protocol
{
AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& pingMessageId)
    : MessageWithGossipsBase(ACK, from, to, gossips)
    , pingMessageId(pingMessageId)
{
}

AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id, const std::string& pingMessageId)
    : MessageWithGossipsBase(ACK, from, to, id, gossips)
    , pingMessageId(pingMessageId)
{
}

gen::membership_protocol::MessageType AckMessage::getProtobufMessageType() const
{
    return gen::membership_protocol::ACK;
}

gen::Message AckMessage::serializeToProtobuf() const
{
    auto message = MessageWithGossipsBase::serializeToProtobuf();
    auto membershipProtocolMessage = getMembershipProtocolMessage(message);

    auto ackFields = std::make_unique<gen::membership_protocol::AckFields>();
    ackFields->set_originalid(pingMessageId);
    membershipProtocolMessage->set_allocated_ackfields(ackFields.release());

    return message;
}

const std::string& AckMessage::getPingMessageId() const
{
    return pingMessageId;
}
}

#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
MessageWithGossipsBase::MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips)
    : Message(msgType, from, to)
    , gossips(gossips)
{
}

MessageWithGossipsBase::MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips)
    : Message(msgType, from, to, id)
    , gossips(gossips)
{
}

gen::Message MessageWithGossipsBase::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();
    auto membershipProtocolMessage = getMembershipProtocolMessage(message);

    for (auto it = gossips.begin(); it != gossips.end(); ++it)
    {
        auto gossip = membershipProtocolMessage->add_gossips();
        it->serializeTo(gossip);
    }
    return message;
}

const std::vector<Gossip>& MessageWithGossipsBase::getGossips() const
{
    return gossips;
}

std::vector<Gossip> MessageWithGossipsBase::parseGossips(const gen::Message& message)
{
    std::vector<Gossip> result;
    auto membershipProtocolMessage = getMembershipProtocolMessage(message);

    result.reserve(membershipProtocolMessage.gossips_size());

    for (int i = 0; i < membershipProtocolMessage.gossips_size(); ++i)
    {
        auto gossip = membershipProtocolMessage.gossips(i);
        network::Address address(gossip.address());
        result.emplace_back(address, MembershipUpdateType(gossip.event()), gossip.id());
    }

    return result;
}
}
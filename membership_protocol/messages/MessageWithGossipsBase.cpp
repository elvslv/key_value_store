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

gen::membership_protocol::Message MessageWithGossipsBase::serializeToProtobuf() const
{
    auto message = Message::serializeToProtobuf();

    for (auto it = gossips.begin(); it != gossips.end(); ++it)
    {
        auto gossip = message.add_gossips();
        it->serializeTo(gossip);
    }
    return message;
}

const std::vector<Gossip>& MessageWithGossipsBase::getGossips() const
{
    return gossips;
}

std::vector<Gossip> MessageWithGossipsBase::parseGossips(const gen::membership_protocol::Message& message)
{
    std::vector<Gossip> result;
    result.reserve(message.gossips_size());

    for (int i = 0; i < message.gossips_size(); ++i)
    {
        auto gossip = message.gossips(i);
        result.emplace_back(network::Address(gossip.address()), MembershipUpdateType(gossip.event()), gossip.id());
    }

    return result;
}
}
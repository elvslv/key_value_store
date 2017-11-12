#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
    MessageWithGossipsBase::MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips): 
        Message(msgType, from, to),
        gossips(gossips)
    {
    }

    MessageWithGossipsBase::MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips): 
        Message(msgType, from, to, id),
        gossips(gossips)
    {
    }

    gen::Message MessageWithGossipsBase::serializeToProtobuf() const
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
}
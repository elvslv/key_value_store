#include "GossipMessage.h"

/*namespace membership_protocol
{
    GossipMessage::GossipMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips): 
        Message(GOSSIP, from, to),
        gossips(gossips)
    {
    }

    GossipMessage::GossipMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips): 
        Message(GOSSIP, from, to, id),
        gossips(gossips)
    {
    }

    gen::Message GossipMessage::serializeToProtobuf() const
    {
        auto message = Message::serializeToProtobuf();

        auto gossipFields = std::make_unique<gen::GossipFields>();
        for (auto it = gossips.begin(); it != gossips.end(); ++it)
        {
            auto gossip = gossipFields->add_gossips();
            it->serializeTo(gossip);
        }

        message.set_allocated_gossipfields(gossipFields.release());

        return message;
    }

    gen::MessageType GossipMessage::getProtobufMessageType() const
    {
        return gen::GOSSIP;
    }

    const std::vector<Gossip>& GossipMessage::getGossips() const
    {
        return gossips;
    }
}*/
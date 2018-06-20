#pragma once

#include "Gossip.h"
#include "Message.h"

/*namespace membership_protocol
{
    class GossipMessage : public Message
    {
    public:
        GossipMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
        GossipMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips);

        const std::vector<Gossip>& getGossips() const;
    protected:
        virtual gen::membership_protocol::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;
        
    private:
        std::vector<Gossip> gossips;
    };
}*/
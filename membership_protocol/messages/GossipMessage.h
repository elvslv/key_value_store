#pragma once

#include "Message.h"
#include "Gossip.h"

namespace membership_protocol
{
    class GossipMessage : public Message
    {
    public:
        GossipMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
        GossipMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;
        
    private:
        std::vector<Gossip> gossips;
    };
}
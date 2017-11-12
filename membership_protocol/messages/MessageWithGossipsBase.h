#pragma once

#include "Message.h"
#include "Gossip.h"

namespace membership_protocol
{
    class MessageWithGossipsBase : public Message
    {
    public:
        MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
        MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips);

        const std::vector<Gossip>& getGossips() const;
    protected:
        virtual gen::Message serializeToProtobuf() const;
        
    private:
        std::vector<Gossip> gossips;
    };
}
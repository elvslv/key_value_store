#pragma once

#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
    class PingMessage : public MessageWithGossipsBase
    {
    public:
        PingMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
        PingMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id);        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
    };
}
#pragma once

#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
    class AckMessage : public MessageWithGossipsBase
    {
    public:
        AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& pingMessageId);
        AckMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id, const std::string& pingMessageId);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;
        
    private:
        std::string pingMessageId;
    };
}
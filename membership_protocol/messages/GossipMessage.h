#pragma once

#include "Message.h"

namespace membership_protocol
{
    class GossipMessage : public Message
    {
    public:
        GossipMessage(const network::Address& from, const network::Address& to, const std::string& pingMessageId);
        GossipMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::string& pingMessageId);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;

    private:
        std::string pingMessageId;
    };
}
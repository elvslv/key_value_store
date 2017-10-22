#pragma once
#include "Message.h"

namespace membership_protocol
{
    class JoinRepMessage: public Message
    {
    public:
        JoinRepMessage(const network::Address& from, const network::Address& to);
        JoinRepMessage(const network::Address& from, const network::Address& to, const std::string& id);        
        virtual gen::MessageType getProtobufMessageType() const;
    };
}

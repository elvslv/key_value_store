#pragma once

#include "Message.h"

namespace membership_protocol
{
    class PingMessage : public Message
    {
    public:
        PingMessage(const network::Address& from, const network::Address& to);
        PingMessage(const network::Address& from, const network::Address& to, const std::string& id);        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
    };
}
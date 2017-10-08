#pragma once

#include "Message.h"

namespace membership_protocol
{
    class AckMessage : public Message
    {
    public:
        AckMessage(const network::Address& from, const network::Address& to, const std::string& pingMessageId);
        AckMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::string& pingMessageId);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;

    private:
        std::string pingMessageId;
    };
}
#pragma once
#include "network/Address.h"
#include "Message.h"

namespace membership_protocol
{
    class JoinReqMessage: public Message
    {
    public:
        JoinReqMessage(const network::Address& from, const network::Address& to);
        JoinReqMessage(const network::Address& from, const network::Address& to, const std::string& id);
        
        virtual gen::MessageType getProtobufMessageType() const;
    };
}

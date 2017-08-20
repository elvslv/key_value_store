#pragma once
#include "../network/Address.h"
#include "Message.h"

namespace membership_protocol
{
    class JoinReqMessage: public Message
    {
    public:
        JoinReqMessage(const network::Address& from, const network::Address& to);
        virtual gen::MessageType getProtobufMessageType();
    };
}

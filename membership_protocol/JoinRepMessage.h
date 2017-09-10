#pragma once
#include "Message.h"
#include "Node.h"

namespace membership_protocol
{
    class JoinRepMessage: public Message
    {
    public:
        JoinRepMessage(const network::Address& from, const network::Address& to);
        std::string toString();
        virtual gen::MessageType getProtobufMessageType();
        virtual gen::Message serializeToProtobuf();
    };
}

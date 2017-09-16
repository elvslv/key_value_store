#pragma once

#include "Message.h"
#include "Events.h"

namespace membership_protocol
{
    class PingMessageBase : public Message
    {
    public:
        virtual std::string toString();

    protected:
        PingMessageBase(MsgTypes msgType, const network::Address& from, const network::Address& to);
        virtual gen::Message serializeToProtobuf();
        virtual gen::MessageType getProtobufMessageType() = 0;
    };
}
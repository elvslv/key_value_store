#pragma once

#include "Message.h"
#include "Events.h"

namespace membership_protocol
{
    class PingMessageBase : public Message
    {
    protected:
        PingMessageBase(MsgTypes msgType, const network::Address& from, const network::Address& to);
        virtual gen::MessageType getProtobufMessageType() = 0;
    };
}
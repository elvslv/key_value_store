#pragma once

#include "PingMessageBase.h"
#include "Events.h"

namespace membership_protocol
{
    class PingMessage : public PingMessageBase
    {
    public:
        PingMessage(const network::Address& from, const network::Address& to);
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
    };
}
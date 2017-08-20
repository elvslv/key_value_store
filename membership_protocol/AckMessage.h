#pragma once

#include "PingMessageBase.h"
#include "Events.h"

namespace membership_protocol
{
    class AckMessage : public PingMessageBase
    {
    public:
        AckMessage(const network::Address& from, const network::Address& to, const std::vector<Event>& ev);
        AckMessage(const network::Address& from, const network::Address& to);

    protected:
        virtual gen::MessageType getProtobufMessageType();
    };
}
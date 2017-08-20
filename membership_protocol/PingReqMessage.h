#pragma once

#include "PingMessageBase.h"
#include "Events.h"

namespace membership_protocol
{
    class PingReqMessage : public PingMessageBase
    {
    public:
        PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress, const std::vector<Event>& ev);
        PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress);
    protected:
        virtual gen::MessageType getProtobufMessageType();
        virtual gen::Message serializeToProtobuf();

    private:
        network::Address targetAddress;
    };
}

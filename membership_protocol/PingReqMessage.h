#pragma once

#include "PingMessageBase.h"
#include "Events.h"

namespace membership_protocol
{
    class PingReqMessage : public PingMessageBase
    {
    public:
        PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress);
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;

    private:
        network::Address targetAddress;
    };
}

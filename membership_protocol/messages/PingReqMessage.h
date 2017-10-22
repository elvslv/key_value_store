#pragma once

#include "Message.h"

namespace membership_protocol
{
    class PingReqMessage : public Message
    {
    public:
        PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress);
        PingReqMessage(const network::Address& from, const network::Address& to, const std::string& id, const network::Address& tgtAddress);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;

    private:
        network::Address targetAddress;
    };
}

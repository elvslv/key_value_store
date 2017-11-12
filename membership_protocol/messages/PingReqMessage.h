#pragma once

#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
    class PingReqMessage : public MessageWithGossipsBase
    {
    public:
        PingReqMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const network::Address& tgtAddress);
        PingReqMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips, const network::Address& tgtAddress);
        
    protected:
        virtual gen::MessageType getProtobufMessageType() const;
        virtual gen::Message serializeToProtobuf() const;

    private:
        network::Address targetAddress;
    };
}

#include "PingReqMessage.h"

namespace membership_protocol
{
    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const network::Address& tgtAddress): 
        MessageWithGossipsBase(PING_REQ, from, to, gossips),
        targetAddress(tgtAddress)
    {
    }

    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips, const network::Address& tgtAddress): 
        MessageWithGossipsBase(PING_REQ, from, to, id, gossips),
        targetAddress(tgtAddress)
    {
    }

    gen::Message PingReqMessage::serializeToProtobuf() const
    {
        auto message = MessageWithGossipsBase::serializeToProtobuf();

        auto addr = targetAddress.serialize();
        auto pingReqFields = std::make_unique<gen::PingReqFields>();
        pingReqFields->set_allocated_targetaddress(addr.release());
        message.set_allocated_pingreqfields(pingReqFields.release());

        return message;
    }

    gen::MessageType PingReqMessage::getProtobufMessageType() const
    {
        return gen::PING_REQ;
    }
}
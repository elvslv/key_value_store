#include "PingReqMessage.h"

namespace membership_protocol
{
    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress, const std::vector<Event>& ev): 
        PingMessageBase(PING_REQ, from, to, ev),
        targetAddress(tgtAddress)
    {
    }

    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress): 
        PingReqMessage(from, to, tgtAddress, std::vector<Event>())
    {
    }

    gen::Message PingReqMessage::serializeToProtobuf()
    {
        auto message = PingMessageBase::serializeToProtobuf();
        auto addr = targetAddress.serialize();
        message.set_allocated_targetaddress(addr.release());

        return message;
    }

    gen::MessageType PingReqMessage::getProtobufMessageType()
    {
        return gen::PING_REQ;
    }
}
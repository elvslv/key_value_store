#include "PingReqMessage.h"

namespace membership_protocol
{
    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const network::Address& tgtAddress): 
        Message(PING_REQ, from, to),
        targetAddress(tgtAddress)
    {
    }

    PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const std::string& id, const network::Address& tgtAddress): 
        Message(PING_REQ, from, to, id),
        targetAddress(tgtAddress)
    {
    }

    gen::Message PingReqMessage::serializeToProtobuf() const
    {
        auto message = Message::serializeToProtobuf();
        auto addr = targetAddress.serialize();
        message.set_allocated_targetaddress(addr.release());

        return message;
    }

    gen::MessageType PingReqMessage::getProtobufMessageType() const
    {
        return gen::PING_REQ;
    }
}
#include "PingReqMessage.h"
#include <sstream>

namespace membership_protocol
{
PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const network::Address& tgtAddress)
    : MessageWithGossipsBase(PING_REQ, from, to, gossips)
    , targetAddress(tgtAddress)
{
}

PingReqMessage::PingReqMessage(const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips, const network::Address& tgtAddress)
    : MessageWithGossipsBase(PING_REQ, from, to, id, gossips)
    , targetAddress(tgtAddress)
{
}

gen::membership_protocol::Message PingReqMessage::serializeToProtobuf() const
{
    auto message = MessageWithGossipsBase::serializeToProtobuf();

    auto addr = targetAddress.serialize();
    auto pingReqFields = std::make_unique<gen::membership_protocol::PingReqFields>();
    pingReqFields->set_allocated_targetaddress(addr.release());
    message.set_allocated_pingreqfields(pingReqFields.release());

    return message;
}

gen::membership_protocol::MessageType PingReqMessage::getProtobufMessageType() const
{
    return gen::membership_protocol::PING_REQ;
}

std::string PingReqMessage::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr(PING_REQ) << " from " << sourceAddress << " to " << destinationAddress << " id " << getId() << " about " << targetAddress << std::endl;
    return ss.str();
}

const network::Address& PingReqMessage::getTargetAddress() const
{
    return targetAddress;
}
}
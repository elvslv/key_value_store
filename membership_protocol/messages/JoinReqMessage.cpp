#include "JoinReqMessage.h"
#include <sstream>

namespace membership_protocol
{
JoinReqMessage::JoinReqMessage(const network::Address& from, const network::Address& to)
    : Message(JOINREQ, from, to)
{
}

JoinReqMessage::JoinReqMessage(const network::Address& from, const network::Address& to, const std::string& id)
    : Message(JOINREQ, from, to, id)
{
}

gen::membership_protocol::MessageType JoinReqMessage::getProtobufMessageType() const
{
    return gen::membership_protocol::JOINREQ;
}
}
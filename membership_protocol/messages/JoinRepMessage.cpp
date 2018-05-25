#include "JoinRepMessage.h"
#include "proto/MembershipProtocolMessage.pb.h"
#include <sstream>

namespace membership_protocol
{
JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips)
    : MessageWithGossipsBase(JOINREP, from, to, gossips)
{
}

JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id)
    : MessageWithGossipsBase(JOINREP, from, to, id, gossips)
{
}

gen::membership_protocol::MessageType JoinRepMessage::getProtobufMessageType() const
{
    return gen::membership_protocol::JOINREP;
}
}
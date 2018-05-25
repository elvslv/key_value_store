#pragma once
#include "MessageWithGossipsBase.h"

namespace membership_protocol
{
class JoinRepMessage : public MessageWithGossipsBase
{
public:
    JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
    JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id);
    virtual gen::membership_protocol::MessageType getProtobufMessageType() const;
};
}

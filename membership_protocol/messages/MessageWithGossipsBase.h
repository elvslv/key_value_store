#pragma once

#include "Gossip.h"
#include "Message.h"

namespace membership_protocol
{
class MessageWithGossipsBase : public Message
{
public:
    MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips);
    MessageWithGossipsBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::string& id, const std::vector<Gossip>& gossips);

    const std::vector<Gossip>& getGossips() const;
    static std::vector<Gossip> parseGossips(const gen::membership_protocol::Message& message);

protected:
    virtual gen::membership_protocol::Message serializeToProtobuf() const;

private:
    std::vector<Gossip> gossips;
};
}
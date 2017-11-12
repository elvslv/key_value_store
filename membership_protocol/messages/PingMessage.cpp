#include "PingMessage.h"

namespace membership_protocol
{
    PingMessage::PingMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips) : 
        MessageWithGossipsBase(PING, from, to, gossips)
    {
    }

    PingMessage::PingMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id) : 
        MessageWithGossipsBase(PING, from, to, id, gossips)
    {
    }

    gen::MessageType PingMessage::getProtobufMessageType() const
    {
        return gen::PING;
    }
}

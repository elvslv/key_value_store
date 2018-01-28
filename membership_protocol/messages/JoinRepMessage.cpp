#include <sstream>
#include "JoinRepMessage.h"
#include "proto/Message.pb.h"

namespace membership_protocol
{
    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips):
        MessageWithGossipsBase(JOINREP, from, to, gossips)
    {
    }

    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Gossip>& gossips, const std::string& id):
        MessageWithGossipsBase(JOINREP, from, to, id, gossips)
    {
    }

    gen::MessageType JoinRepMessage::getProtobufMessageType() const
    {
        return gen::JOINREP;
    }
}
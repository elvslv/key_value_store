#include <sstream>
#include "JoinRepMessage.h"
#include "proto/Message.pb.h"

namespace membership_protocol
{
    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to):
        Message(JOINREP, from, to)
    {
    }

    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::string& id):
        Message(JOINREP, from, to, id)
    {
    }

    gen::MessageType JoinRepMessage::getProtobufMessageType() const
    {
        return gen::JOINREP;
    }
}
#include <sstream>
#include "JoinRepMessage.h"
#include "../proto/Message.pb.h"

namespace membership_protocol
{
    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to):
        Message(JOINREP, from, to)
    {
    }
    gen::MessageType JoinRepMessage::getProtobufMessageType() const
    {
        return gen::JOINREP;
    }
}
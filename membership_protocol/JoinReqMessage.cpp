#include <sstream>
#include "JoinReqMessage.h"

namespace membership_protocol
{
    JoinReqMessage::JoinReqMessage(const network::Address& from, const network::Address& to):
        Message(JOINREQ, from, to)
    {
    }

    gen::MessageType JoinReqMessage::getProtobufMessageType() const
    {
        return gen::JOINREQ;
    }
}
#include <sstream>
#include "JoinReqMessage.h"

namespace membership_protocol
{
    JoinReqMessage::JoinReqMessage(const network::Address& from, const network::Address& to):
        Message(JOINREQ, from, to)
    {
    }

    JoinReqMessage::JoinReqMessage(const network::Address& from, const network::Address& to, const std::string& id):
        Message(JOINREQ, from, to, id)
    {
    }

    gen::MessageType JoinReqMessage::getProtobufMessageType() const
    {
        return gen::JOINREQ;
    }
}
#include "PingMessage.h"

namespace membership_protocol
{
    PingMessage::PingMessage(const network::Address& from, const network::Address& to)
        : PingMessageBase(PING, from, to)
    {
    }

    gen::MessageType PingMessage::getProtobufMessageType() const
    {
        return gen::PING;
    }
}

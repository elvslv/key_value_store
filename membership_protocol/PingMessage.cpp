#include "PingMessage.h"

namespace membership_protocol
{
    PingMessage::PingMessage(const network::Address& from, const network::Address& to, const std::vector<Event>& ev)
        : PingMessageBase(PING, from, to, ev)
    {
    }

    PingMessage::PingMessage(const network::Address& from, const network::Address& to)
        : PingMessage(from, to, std::vector<Event>())
    {
    }

    gen::MessageType PingMessage::getProtobufMessageType()
    {
        return gen::PING;
    }
}

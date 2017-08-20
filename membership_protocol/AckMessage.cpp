#include "AckMessage.h"

namespace membership_protocol
{
    AckMessage::AckMessage(const network::Address& from, const network::Address& to, const std::vector<Event>& ev)
        : PingMessageBase(ACK, from, to, ev)
    {
    }

    AckMessage::AckMessage(const network::Address& from, const network::Address& to)
        : AckMessage(from, to, std::vector<Event>())
    {
    }

    gen::MessageType AckMessage::getProtobufMessageType()
    {
        return gen::ACK;
    }
}

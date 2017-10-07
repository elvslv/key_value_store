#include "AckMessage.h"

namespace membership_protocol
{
    AckMessage::AckMessage(const network::Address& from, const network::Address& to)
        : PingMessageBase(ACK, from, to)
    {
    }

    gen::MessageType AckMessage::getProtobufMessageType() const
    {
        return gen::ACK;
    }
}

#include "PingMessage.h"

namespace membership_protocol
{
    PingMessage::PingMessage(const network::Address& from, const network::Address& to) : 
        Message(PING, from, to)
    {
    }

    PingMessage::PingMessage(const network::Address& from, const network::Address& to, const std::string& id) : 
        Message(PING, from, to, id)
    {
    }

    gen::MessageType PingMessage::getProtobufMessageType() const
    {
        return gen::PING;
    }
}

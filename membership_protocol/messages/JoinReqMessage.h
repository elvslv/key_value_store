#pragma once
#include "Message.h"
#include "network/Address.h"

namespace membership_protocol
{
class JoinReqMessage : public Message
{
public:
    JoinReqMessage(const network::Address& from, const network::Address& to);
    JoinReqMessage(const network::Address& from, const network::Address& to, const std::string& id);

    virtual gen::membership_protocol::MessageType getProtobufMessageType() const;
};
}

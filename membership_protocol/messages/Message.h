#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/MembershipProtocolMessage.pb.h"
#include "utils/Message.h"
#include <iostream>
#include <memory>
#include <string>

namespace membership_protocol
{
class Message : public utils::Message
{
public:
    enum MsgTypes
    {
        JOINREQ,
        JOINREP,
        PING,
        ACK,
        PING_REQ,
        // DUMMYLASTMSGTYPE
    };

    virtual ~Message(){};

    MsgTypes getMessageType() const;
    virtual std::string toString() const;

protected:
    Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress);
    Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress, const std::string& id);
    virtual gen::membership_protocol::MessageType getProtobufMessageType() const = 0;

    static gen::membership_protocol::Message* getMembershipProtocolMessage(gen::Message& message);
    static const gen::membership_protocol::Message& getMembershipProtocolMessage(const gen::Message& message);

    virtual gen::Message serializeToProtobuf() const;

private:
    MsgTypes messageType;
};
}

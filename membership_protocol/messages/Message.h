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
    std::string getMessageTypeDescription() const;

    static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage);
    virtual std::string toString() const;
    virtual network::Message serialize() const;

protected:
    Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress);
    Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress, const std::string& id);
    virtual gen::membership_protocol::Message serializeToProtobuf() const;
    virtual gen::membership_protocol::MessageType getProtobufMessageType() const = 0;
    static std::string getMsgTypeStr(MsgTypes msgType);

private:
    MsgTypes messageType;
};
}

#pragma once

#include <iostream>
#include <memory>
#include "../proto/Message.pb.h"
#include "../network/Address.h"
#include "../network/Message.h"

namespace membership_protocol
{
    enum MsgTypes
    {
        JOINREQ,
        JOINREP,
        PING,
        ACK,
        PING_REQ,
        DUMMYLASTMSGTYPE
    };

    class Message
    {
    public:
        virtual ~Message();

        MsgTypes getMessageType() const;

        static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage); 
        virtual std::string toString();
        virtual network::Message serialize();
        const network::Address& getSourceAddress();
        const network::Address& getDestinationAddress();
    protected:
        Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress);
        virtual gen::Message serializeToProtobuf();
        virtual gen::MessageType getProtobufMessageType() = 0;
        static std::string getMsgTypeStr(MsgTypes msgType);

        network::Address from;
        network::Address to;
    private:
        MsgTypes messageType;
    };
}


#pragma once

#include <iostream>
#include <memory>
#include <string>
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
        GOSSIP, 
        // DUMMYLASTMSGTYPE
    };

    class Message
    {
    public:
        virtual ~Message();
        Message(Message&&) = default;
        Message& operator = (Message&&) = default;
    
        Message(const Message&) = default;
        Message& operator = (const Message&) = default;
    
        MsgTypes getMessageType() const;

        static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage); 
        virtual std::string toString() const;
        virtual network::Message serialize() const;
        const network::Address& getSourceAddress() const;
        const network::Address& getDestinationAddress() const;
        const std::string& getId() const;
    protected:
        Message(MsgTypes messageType, const network::Address& srcAddress, const network::Address& destAddress);
        virtual gen::Message serializeToProtobuf() const;
        virtual gen::MessageType getProtobufMessageType() const = 0;
        static std::string getMsgTypeStr(MsgTypes msgType);

        network::Address from;
        network::Address to;
    private:
        MsgTypes messageType;
        std::string id;
    };
}


#pragma once

#include "Message.h"
#include "network/Address.h"
#include "network/Message.h"
#include "proto/KeyValueStoreMessage.pb.h"

#include <memory>

namespace key_value_store
{

class RequestMessage : public Message
{
public:
    enum MsgTypes
    {
        CREATE,
        UPDATE,
        READ,
        DELETE,
    };

    virtual ~RequestMessage(){};

    MsgTypes getMessageType() const;
    std::string getKey() const;

    virtual std::string toString() const;
    virtual network::Message serialize() const;

protected:
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::Message serializeToProtobuf() const;
    gen::RequestMessageType getProtobufMessageType() const;
    static std::string getMsgTypeStr(MsgTypes msgType);

    static gen::RequestMessage getRequestMessage(const gen::Message& message);

private:
    MsgTypes messageType;
    std::string key;
};
}
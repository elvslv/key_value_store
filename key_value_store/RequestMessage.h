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

protected:
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::Message serializeToProtobuf() const = 0;
    gen::RequestMessageType getProtobufMessageType() const;
    virtual std::string getMsgTypeStr() const;

    static gen::RequestMessage getRequestMessage(const gen::Message& message);

private:
    MsgTypes messageType;
    std::string key;
};
}
#pragma once

#include "Message.h"
#include "network/Address.h"
#include "network/Message.h"
#include "proto/KeyValueStoreMessage.pb.h"

#include <memory>

namespace key_value_store
{

class ResponseMessage : public Message
{
public:
    enum MsgTypes
    {
        CREATE,
        UPDATE,
        READ,
        DELETE,
    };

    virtual ~ResponseMessage(){};

    MsgTypes getMessageType() const;
    std::string getKey() const;

protected:
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::Message serializeToProtobuf() const;
    gen::ResponseMessageType getProtobufMessageType() const;
    virtual std::string getMsgTypeStr() const;
    static gen::ResponseMessage getResponseMessage(const gen::Message& message);

private:
    MsgTypes messageType;
    std::string key;
};
}
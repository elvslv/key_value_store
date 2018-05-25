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
    virtual ~RequestMessage(){};

    std::string getKey() const;

    virtual bool isRequest() const { return true; }

protected:
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::key_value_store::Message serializeToProtobuf() const = 0;
    gen::key_value_store::RequestMessageType getProtobufMessageType() const;
    virtual std::string getMsgTypeStr() const;

    static gen::key_value_store::RequestMessage* getRequestMessage(gen::key_value_store::Message& message);

private:
    std::string key;
};
}
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
    unsigned long getTimestamp() const;

    virtual bool isRequest() const { return true; }

protected:
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id, unsigned long timestamp);
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::Message serializeToProtobuf() const = 0;
    gen::key_value_store::RequestMessageType getProtobufMessageType() const;

    static gen::key_value_store::RequestMessage* getRequestMessage(gen::Message& message);

private:
    std::string key;
    unsigned long timestamp;
};
}
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
    virtual ~ResponseMessage(){};

    std::string getOriginalMessageId() const;
    unsigned int getResponseCode() const;

    virtual bool isRequest() const { return false; }

protected:
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id);
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode);

    virtual gen::Message serializeToProtobuf() const = 0;
    gen::key_value_store::ResponseMessageType getProtobufMessageType() const;
    static gen::key_value_store::ResponseMessage* getResponseMessage(gen::Message& message);

private:
    std::string originalMessageId;
    unsigned int responseCode;
};
}
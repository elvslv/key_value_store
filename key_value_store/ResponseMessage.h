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

    MsgTypes getMessageType() const;
    std::string getOriginalMessageId() const;
    unsigned int getResponseCode() const;

    virtual bool isRequest() const { return false; }

protected:
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode, const std::string& id);
    ResponseMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& originalMessageId, unsigned int responseCode);

    virtual gen::Message serializeToProtobuf() const = 0;
    gen::ResponseMessageType getProtobufMessageType() const;
    virtual std::string getMsgTypeStr() const;
    static gen::ResponseMessage getResponseMessage(const gen::Message& message);

private:
    MsgTypes messageType;
    std::string originalMessageId;
    unsigned int responseCode;
};
}
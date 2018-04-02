#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/KeyValueStoreMessage.pb.h"
#include "utils/Message.h"

#include <memory>

namespace key_value_store
{

class RequestMessage : public utils::Message
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
    network::Address getDestinationAddress() const;
    network::Address getSourceAddress() const;
    std::string getKey() const;
    std::string getId() const;

    static std::unique_ptr<RequestMessage> parseMessage(const network::Message& networkMessage);
    virtual std::string toString() const;
    virtual network::Message serialize() const;

protected:
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    RequestMessage(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::RequestMessage serializeToProtobuf() const;
    gen::RequestMessageType getProtobufMessageType() const;
    static std::string getMsgTypeStr(MsgTypes msgType);

private:
    MsgTypes messageType;
    std::string key;
};
}
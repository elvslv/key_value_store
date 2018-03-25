#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/KeyValueStoreMessage.pb.h"

#include <memory>

namespace key_value_store
{

enum RequestMessageType
{
    CREATE,
    UPDATE,
    READ,
    DELETE,
};

class RequestMessage
{
public:
    virtual ~RequestMessage(){};
    RequestMessage(RequestMessage&&) = default;
    RequestMessage& operator=(RequestMessage&&) = default;

    RequestMessage(const RequestMessage&) = default;
    RequestMessage& operator=(const RequestMessage&) = default;

    RequestMessageType getMessageType() const;
    network::Address getDestinationAddress() const;
    network::Address getSourceAddress() const;
    std::string getKey() const;
    std::string getId() const;

    static std::unique_ptr<RequestMessage> parseMessage(const network::Message& networkMessage);
    virtual std::string toString() const;
    virtual network::Message serialize() const;

protected:
    RequestMessage(RequestMessageType messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    RequestMessage(RequestMessageType messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::RequestMessage serializeToProtobuf() const;
    gen::RequestMessageType getProtobufMessageType() const;
    static std::string getMsgTypeStr(RequestMessageType msgType);

private:
    RequestMessageType messageType;
    network::Address sourceAddress;
    network::Address destinationAddress;
    std::string key;
    std::string id;
};
}
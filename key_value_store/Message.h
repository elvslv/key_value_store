#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/KeyValueStoreMessage.pb.h"
#include "utils/Message.h"

#include <memory>

namespace key_value_store
{
class Message : public utils::Message
{
public:
    enum MsgTypes
    {
        CREATE_REQUEST = 100,
        UPDATE_REQUEST,
        READ_REQUEST,
        DELETE_REQUEST,

        CREATE_RESPONSE = 200,
        UPDATE_RESPONSE,
        READ_RESPONSE,
        DELETE_RESPONSE,
    };

    virtual ~Message(){};

    static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage);

    network::Message serialize() const;
    virtual std::string toString() const;
    MsgTypes getMessageType() const;
    virtual std::string getMessageTypeDescription() const;

    virtual bool isRequest() const = 0;

protected:
    Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id);
    Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress);

    virtual gen::key_value_store::Message serializeToProtobuf() const;
    virtual std::string getMsgTypeStr() const = 0;

private:
    MsgTypes messageType;
};
}
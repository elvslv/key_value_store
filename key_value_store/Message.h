#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/WrapperMessage.pb.h"
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
        REPAIR_REQUEST,

        CREATE_RESPONSE = 200,
        UPDATE_RESPONSE,
        READ_RESPONSE,
        DELETE_RESPONSE,
    };

    virtual ~Message(){};

    virtual std::string toString() const;
    MsgTypes getMessageType() const;

    virtual bool isRequest() const = 0;

protected:
    Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id);
    Message(MsgTypes messageType, const network::Address& sourceAddress, const network::Address& destinationAddress);

    static gen::key_value_store::Message* getKeyValueStoreMessage(gen::Message& message);
    virtual gen::Message serializeToProtobuf() const;

private:
    MsgTypes messageType;
};
}
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
    virtual ~Message(){};

    static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage);

    network::Message serialize() const;
    virtual std::string toString() const;

protected:
    Message(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id);
    Message(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key);

    virtual gen::Message serializeToProtobuf() const;
    virtual std::string getMsgTypeStr() const = 0;
};
}
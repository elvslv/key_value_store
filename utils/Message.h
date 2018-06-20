#pragma once

#include "network/Address.h"
#include "network/Message.h"
#include "proto/WrapperMessage.pb.h"

#include <string>

namespace utils
{
class Message
{
public:
    virtual ~Message() {}

    Message(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id);
    Message(const network::Address& sourceAddress, const network::Address& destinationAddress);

    Message(Message&&) = default;
    Message& operator=(Message&&) = default;

    Message(const Message&) = default;
    Message& operator=(const Message&) = default;

    network::Address getDestinationAddress() const;
    network::Address getSourceAddress() const;
    std::string getId() const;

    virtual std::string toString() const = 0;
    network::Message serialize() const;

    std::string getMsgTypeStr() const;

    template <typename MessageT>
    static std::string getTypeName()
    {
        return typeid(MessageT).name();
    }

    static std::unique_ptr<Message> parseMessage(const network::Message& networkMessage);

protected:
    virtual gen::Message serializeToProtobuf() const;

    network::Address sourceAddress;
    network::Address destinationAddress;
    std::string id;
};
}
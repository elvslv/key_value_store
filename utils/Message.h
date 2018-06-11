#pragma once

#include "network/Address.h"
#include "network/Message.h"

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
    virtual network::Message serialize() const = 0;

    virtual std::string getMessageTypeDescription() const = 0;

protected:
    network::Address sourceAddress;
    network::Address destinationAddress;
    std::string id;
};
}
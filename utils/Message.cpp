#include "Message.h"
#include "Utils.h"

namespace utils
{
Message::Message(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id)
    : sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , id(id)
{
}

Message::Message(const network::Address& sourceAddress, const network::Address& destinationAddress)
    : sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , id(utils::Utils::getRandomString(16))
{
}

network::Address Message::getSourceAddress() const
{
    return sourceAddress;
}

network::Address Message::getDestinationAddress() const
{
    return destinationAddress;
}

std::string Message::getId() const
{
    return id;
}
}
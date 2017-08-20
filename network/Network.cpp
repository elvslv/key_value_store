#include "EmulNet.h"
#include "Network.h"

namespace network
{
    Network::Network(const Address& addr) :
        address(addr)
    {   
    }

    void Network::send(const Address& destAddr, const Message& message) const
    {
        auto emulNet = EmulNet::instance();
        emulNet->send(address, destAddr, message);
    }

    Message Network::receive() const
    {
        auto emulNet = EmulNet::instance();
        return emulNet->receive(address);
    }
}
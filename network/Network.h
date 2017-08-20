#pragma once

#include "Address.h"
#include "Message.h"

namespace network
{
    class Network
    {
    public:
        Network(const Address& addr);

        void send(const Address& destAddr, const Message& message) const;
        Message receive() const;

    private:
        const Address address;
    };
}
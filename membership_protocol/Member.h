#pragma once

#include <algorithm>
#include "network/Address.h"

namespace membership_protocol
{
    struct Member
    {
        Member():
            address()
        {
        };

        Member(const network::Address& address):
            address(address)
        {
        }

        size_t hash(size_t getHashCode) const
        {
            return std::hash(address.toString()) % getHashCode;
        }

        network::Address address;
    };
}

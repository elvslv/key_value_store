#pragma once

#include "network/Address.h"
#include <algorithm>

namespace membership_protocol
{
struct Member
{
    Member()
        : address(){};

    Member(const network::Address& address)
        : address(address)
    {
    }

    size_t hash(size_t getHashCode) const
    {
        std::hash<std::string> hash_fn;
        return hash_fn(address.toString()) % getHashCode;
    }

    network::Address address;
};
}

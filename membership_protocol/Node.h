#pragma once

#include <functional>
#include <vector>
#include "../network/Address.h"

namespace membership_protocol
{
    class Node
    {
    public:
        Node(const network::Address& addr);
        const network::Address& getAddress() const;
        size_t getHashCode() const;
    private:
        network::Address address;
        std::hash<std::string> hash_fn;
    };
}
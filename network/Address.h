#pragma once

#include <array>
#include <memory>
#include <string>
// #include "../build/gen/Address.pb.h"

namespace network
{
    struct Address
    {
        Address(const std::array<unsigned char, 4>& addr, unsigned short p):
            address(addr),
            port(p)
        {
        }

        // Address(const gen::Address& addr);
        std::string toString() const;
        // std::unique_ptr<gen::Address> serialize() const;

        std::array<unsigned char, 4> address;
        unsigned short port; 
    };
}
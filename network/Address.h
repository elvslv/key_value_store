#pragma once

#include <array>
#include <string>

namespace network
{
    struct Address
    {
        std::array<unsigned char, 4> address;
        unsigned short port; 

        std::string toString() const;
    };
}
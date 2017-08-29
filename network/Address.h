#pragma once

#include <array>
#include <memory>
#include <string>
#include "../proto/Address.pb.h"

namespace network
{
    struct Address
    {
        Address(const std::array<unsigned char, 4>& addr, unsigned short p);
        Address(const std::string& addr);
        Address(const Address& other);
        Address(const gen::Address& addr);
        
        std::string toString() const;
        std::unique_ptr<gen::Address> serialize() const;

        static int parseNextNum(const std::string& addr, char delim, int& start, int& end);

        std::array<unsigned char, 4> address;
        unsigned short port; 
    };
}
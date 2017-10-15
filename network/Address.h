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
        Address(){};

        friend bool operator==(const Address& address1, const Address& address2);
        friend std::ostream& operator<<(std::ostream& os, const Address& address);
        std::string operator()() const;

        std::string toString() const;
        std::unique_ptr<gen::Address> serialize() const;

        static int parseNextNum(const std::string& addr, char delim, int& start, int& end);

        std::array<unsigned char, 4> address;
        unsigned short port; 
    };
}

namespace std 
{
    template <> struct hash<network::Address>
    {
      size_t operator()(const network::Address& address) const
      {
        return hash<std::string>()(address.toString());
      }
    };
  }
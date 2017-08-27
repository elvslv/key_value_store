#include <cassert>
#include <sstream>
#include "Address.h"

namespace network
{
    Address::Address(const Address& other):
        address(other.address),
        port(other.port)
    {
    }

    Address::Address(const std::string& addr)
    {
        int start = 0;
        int end = 0;
        int i = 0;
        for (i = 0; i < 3; ++i)
        {
            address[i] = parseNextNum(addr, '.', start, end);
        }

        address[i] = parseNextNum(addr, ':', start, end);        
        
        port = std::stoi(addr.substr(start));
    }

    Address::Address(const std::array<unsigned char, 4>& addr, unsigned short p):
        address(addr),
        port(p)
    {
    }

    Address::Address(const gen::Address& addr)
    {

    }

    int Address::parseNextNum(const std::string& addr, char delim, int& start, int& end)
    {
        end = addr.find(delim, start);
        assert(end != std::string::npos);

        int result = std::stoi(addr.substr(start, end));            
        start = end + 1;

        return result;
    }

    std::string Address::toString() const
    {
        std::stringstream ss;
        for (auto it = address.begin(); it != address.end(); ++it)
        {
            if (it != address.begin())
            {
                ss << ".";
            }

            ss << *it;
        }

        ss << ":" << port;

        return ss.str();
    }
}
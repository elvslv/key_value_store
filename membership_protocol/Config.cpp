#include "Config.h"

namespace membership_protocol
{
    Config::Config(int n)
    {
        for (int i = 0; i < n; ++i)
        {
            addresses.emplace_back(std::array<unsigned char, 4>{{static_cast<unsigned char>(i + 1), 0, 0, 0}}, (i + 1) * 100);
        }
    }

    Config::Config(const std::vector<network::Address>& addresses):
        addresses(addresses)
    {
    }


    std::vector<network::Address> Config::getAddressesToJoin()
    {
        return addresses;
    }
}
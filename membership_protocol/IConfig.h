#pragma once

#include <vector>
#include "network/Address.h"

namespace membership_protocol 
{
    class IConfig
    {
    public:
        virtual ~IConfig() {}
        virtual std::vector<network::Address> getAddressesToJoin() = 0;
    };
}
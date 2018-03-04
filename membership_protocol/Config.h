#pragma once

#include <vector>
#include "IConfig.h"

namespace membership_protocol 
{
    class Config: IConfig
    {
    public:
        Config(int n);
        Config(const std::vector<network::Address>& addresses);

        virtual ~Config() {}
        virtual std::vector<network::Address> getAddressesToJoin();

    private:
        std::vector<network::Address> addresses;
    };
}
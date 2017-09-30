#pragma once

#include "../network/Address.h"

namespace membership_protocol
{
    struct Member
    {
        Member():
            address()
        {
        };

        Member(const network::Address& address):
            address(address)
        {
        }

        network::Address address;
    };
}

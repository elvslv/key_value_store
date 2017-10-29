#pragma once

#include "Member.h"

namespace membership_protocol
{
    enum MembershipUpdateType
    {
        JOINED,
        FAILED,
    };

    enum MembershipUpdateSource
    {
        FAILURE_DETECTOR,
        GOSSIP_PROTOCOL,
        INITIAL_SYNC,
        MEMBERSHIP_PROTOCOL,
    };

    struct MembershipUpdate 
    {
        MembershipUpdate() {}

        MembershipUpdate(const network::Address& address, const MembershipUpdateType& updateType):
            address(address),
            updateType(updateType)
        {
        }

        network::Address address;
        MembershipUpdateType updateType;
    };
}
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
    };

    struct MembershipUpdate 
    {
        MembershipUpdate(const Member& member, const MembershipUpdateType& updateType):
            member(member),
            updateType(updateType)
        {
        }

        Member member;
        MembershipUpdateType updateType;
    };
}
#pragma once

#include "membership_protocol/MembershipUpdate.h"

namespace gossip_protocol
{
    class IGossipProtocol
    {
    public:
        class IObserver
        {
        public:
            virtual void onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate) = 0;
        };

        virtual ~IGossipProtocol(){};
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void addObserver(IObserver* observer) = 0;
        virtual void spreadMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate) = 0;
    };
}
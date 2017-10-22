#pragma once

#include "GossipEvent.h"
#include "MembershipUpdate.h"

namespace membership_protocol
{
    class IGossipProtocol
    {
    public:
        class IObserver
        {
        public:
            virtual void onGossipEvent(const gossip_protocol::GossipEvent& gossipEvent) = 0;
        };

        virtual ~IGossipProtocol(){};
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void addObserver(IObserver* observer) = 0;
        virtual void spreadMembershipUpdate(const MembershipUpdate& membershipUpdate) = 0;
    };
}
#pragma once

#include "membership_protocol/MembershipUpdate.h"
#include "membership_protocol/messages/Gossip.h"

namespace gossip_protocol
{
    class IGossipProtocol
    {
    public:
        class IObserver
        {
        public:
            virtual ~IObserver(){}
            virtual void onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate) = 0;
        };

        virtual ~IGossipProtocol(){};
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void addObserver(IObserver* observer) = 0;
        virtual void spreadMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate) = 0;
        virtual std::vector<membership_protocol::Gossip> getGossipsForAddress(const network::Address& address) = 0;
        virtual void onNewGossips(const network::Address& sourceAddress, const std::vector<membership_protocol::Gossip>& gossips) = 0;
    };
}
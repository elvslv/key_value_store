#pragma once
#include <memory>
#include "network/Address.h"
#include "proto/Message.pb.h"
#include "membership_protocol/MembershipUpdate.h"

namespace membership_protocol
{
    struct Gossip
    {
        Gossip(const network::Address& address, MembershipUpdateType membershipUpdateType, const std::string& id);

        gen::GossipEventTypes getProtobufEventsType() const;        
        void serializeTo(gen::Gossip* gossip) const;

        network::Address address;
        MembershipUpdateType membershipUpdateType;
        std::string id;
    };
}
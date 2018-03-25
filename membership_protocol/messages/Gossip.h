#pragma once
#include "membership_protocol/MembershipUpdate.h"
#include "network/Address.h"
#include "proto/MembershipProtocolMessage.pb.h"
#include <memory>

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
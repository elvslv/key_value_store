#pragma once
#include <memory>
#include "network/Address.h"
#include "proto/Message.pb.h"

namespace membership_protocol
{
    enum GossipType
    {
        JOINED,
        FAILED,
    };

    struct Gossip
    {
        Gossip(const network::Address& address, GossipType gossipType, const std::string& id);

        gen::GossipEventTypes getProtobufEventsType() const;        
        void serializeTo(gen::Gossip* gossip) const;

        network::Address address;
        GossipType gossipType;
        std::string id;
    };
}
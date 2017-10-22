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
    public:
        Gossip(const network::Address& address, GossipType gossipType, const std::string& id);

        void serializeTo(gen::Gossip* gossip) const;
    private:
        network::Address address;
        GossipType gossipType;
        std::string id;

        gen::GossipEventTypes getProtobufEventsType() const;
    };
}
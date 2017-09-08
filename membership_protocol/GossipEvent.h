#pragma once

#include "../network/Address.h"

namespace gossip_protocol
{
    enum GossipEventType
    {
        JOINED,
        FAILED,
    };

    struct GossipEvent
    {
        network::Address address;
        GossipEventType eventType;
    };
}
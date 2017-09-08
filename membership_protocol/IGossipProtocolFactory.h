#pragma once

#include "IGossipProtocol.h"
#include "IMembershipProtocol.h"
#include "../network/Network.h"
#include "../utils/Log.h"

namespace membership_protocol
{
    class IGossipProtocolFactory
    {
    public:
        virtual std::unique_ptr<IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<network::Network>& network, membership_protocol::IMembershipProtocol* membershipProtocol) = 0;
    };
}
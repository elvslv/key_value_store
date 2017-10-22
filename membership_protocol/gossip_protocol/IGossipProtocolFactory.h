#pragma once

#include "IGossipProtocol.h"
#include "IMembershipProtocol.h"
#include "network/Network.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"

namespace membership_protocol
{
    class IGossipProtocolFactory
    {
    public:
        virtual ~IGossipProtocolFactory(){};        
        virtual std::unique_ptr<IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol) = 0;
    };
}
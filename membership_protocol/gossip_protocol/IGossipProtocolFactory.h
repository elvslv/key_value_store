#pragma once

#include "IGossipProtocol.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "network/Network.h"
#include "utils/IThreadPolicy.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"

namespace gossip_protocol
{
class IGossipProtocolFactory
{
public:
    virtual ~IGossipProtocolFactory(){};
    virtual std::unique_ptr<IGossipProtocol> createGossipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger, membership_protocol::IMembershipProtocol* membershipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy) = 0;
};
}
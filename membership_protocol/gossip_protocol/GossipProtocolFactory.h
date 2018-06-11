#pragma once

#include "membership_protocol/gossip_protocol/IGossipProtocolFactory.h"

namespace gossip_protocol
{
class GossipProtocolFactory : public IGossipProtocolFactory
{
public:
    virtual ~GossipProtocolFactory(){};
    virtual std::unique_ptr<IGossipProtocol> createGossipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger, membership_protocol::IMembershipProtocol* membershipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy);
};
}
#include "membership_protocol/gossip_protocol/GossipProtocolFactory.h"
#include "membership_protocol/gossip_protocol/GossipProtocol.h"

namespace gossip_protocol
{
std::unique_ptr<IGossipProtocol> GossipProtocolFactory::createGossipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger, membership_protocol::IMembershipProtocol* membershipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy)
{
    return std::make_unique<GossipProtocol>(addr, logger, membershipProtocol, threadPolicy);
}
}
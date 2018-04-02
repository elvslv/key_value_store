#pragma once

#include "IFailureDetector.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "membership_protocol/gossip_protocol/IGossipProtocol.h"
#include "membership_protocol/messages/Message.h"
#include "network/Network.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"

namespace failure_detector
{
class IFailureDetectorFactory
{
public:
    virtual ~IFailureDetectorFactory(){};
    virtual std::unique_ptr<IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher<membership_protocol::Message>>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol) = 0;
};
}
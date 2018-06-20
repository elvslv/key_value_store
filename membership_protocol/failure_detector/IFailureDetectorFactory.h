#pragma once

#include "IFailureDetector.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "membership_protocol/gossip_protocol/IGossipProtocol.h"
#include "membership_protocol/messages/Message.h"
#include "network/Network.h"
#include "utils/IThreadPolicy.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"

namespace failure_detector
{
class IFailureDetectorFactory
{
public:
    virtual ~IFailureDetectorFactory(){};
    virtual std::unique_ptr<IFailureDetector> createFailureDetector(const network::Address& addr, std::shared_ptr<utils::Log> logger, std::shared_ptr<utils::MessageDispatcher> messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy) = 0;
};
}
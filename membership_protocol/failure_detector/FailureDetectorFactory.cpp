#include "membership_protocol/failure_detector/FailureDetectorFactory.h"
#include "membership_protocol/failure_detector/FailureDetector.h"

namespace failure_detector
{
std::unique_ptr<IFailureDetector> FailureDetectorFactory::createFailureDetector(const network::Address& addr, std::shared_ptr<utils::Log> logger, std::shared_ptr<utils::MessageDispatcher> messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy)
{
    return std::make_unique<FailureDetector>(addr, logger, messageDispatcher, membershipProtocol, gossipProtocol, threadPolicy);
}
}
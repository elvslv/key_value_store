#pragma once

#include "membership_protocol/failure_detector/IFailureDetectorFactory.h"

namespace failure_detector
{
class FailureDetectorFactory : public IFailureDetectorFactory
{
public:
    virtual ~FailureDetectorFactory(){};
    virtual std::unique_ptr<IFailureDetector> createFailureDetector(const network::Address& addr, std::shared_ptr<utils::Log> logger, std::shared_ptr<utils::MessageDispatcher<membership_protocol::Message>> messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy);
};
}
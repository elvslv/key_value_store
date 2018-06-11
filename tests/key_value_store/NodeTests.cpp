#include "key_value_store/Node.h"
#include "key_value_store/Partitioner.h"
#include "key_value_store/Storage.h"

#include "membership_protocol/MembershipProtocol.h"
#include "membership_protocol/failure_detector/FailureDetectorFactory.h"
#include "membership_protocol/gossip_protocol/GossipProtocolFactory.h"

#include "utils/ThreadPolicy.h"

#include "gtest/gtest.h"

namespace
{
TEST(NodeTests, Constructor)
{
    network::Address addr("1.0.0.0:100");
    auto logger = std::make_shared<utils::Log>();

    size_t ringSize = 512;
    std::unique_ptr<key_value_store::IPartitioner> partitioner = std::make_unique<key_value_store::Partitioner>(ringSize);

    auto membershipProtocolMessageDispatcher = std::make_shared<utils::MessageDispatcher<membership_protocol::Message>>(addr, logger);
    std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<failure_detector::FailureDetectorFactory>();
    std::unique_ptr<gossip_protocol::IGossipProtocolFactory> gossipProtocolFactory = std::make_unique<gossip_protocol::GossipProtocolFactory>();

    auto threadPolicy = std::make_shared<utils::ThreadPolicy>();

    std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol = std::make_unique<membership_protocol::MembershipProtocol>(addr, logger, membership_protocol::Config(1), membershipProtocolMessageDispatcher, failureDetectorFactory, gossipProtocolFactory, threadPolicy);

    std::unique_ptr<key_value_store::IStorage> storage = std::make_unique<key_value_store::Storage>();
    auto keyValueStoreMessageDispatcher = std::make_shared<utils::MessageDispatcher<key_value_store::Message>>(addr, logger);

    key_value_store::Node node(addr, logger, std::move(membershipProtocol), std::move(storage), partitioner, keyValueStoreMessageDispatcher, threadPolicy);
}
}
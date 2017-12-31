#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../mocks/Mocks.h"

#include "membership_protocol/MembershipProtocol.h"
#include "membership_protocol/gossip_protocol/GossipProtocol.h"
#include "membership_protocol/failure_detector/FailureDetector.h"
#include "utils/ThreadPolicy.h"

namespace
{
    class FakeFailureDetectorFactory: public failure_detector::IFailureDetectorFactory
    {
        virtual std::unique_ptr<failure_detector::IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol)
        {
            return std::make_unique<testing::NiceMock<mock::MockIFailureDetector> >();
        }
    };

    class FakeGossipProtocolFactory: public gossip_protocol::IGossipProtocolFactory
    {
        virtual std::unique_ptr<gossip_protocol::IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            return std::make_unique<testing::NiceMock<mock::MockIGossipProtocol> >();
        }
    };

    class FailureDetectorFactory: public failure_detector::IFailureDetectorFactory
    {
        virtual std::unique_ptr<failure_detector::IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol)
        {
            std::unique_ptr<utils::IThreadPolicy> threadPolicy = std::make_unique<utils::ThreadPolicy>();
            return std::make_unique<failure_detector::FailureDetector>(addr, logger, messageDispatcher, membershipProtocol, gossipProtocol, threadPolicy);
        }
    }; 

    class GossipProtocolFactory: public gossip_protocol::IGossipProtocolFactory
    {
        virtual std::unique_ptr<gossip_protocol::IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            std::unique_ptr<utils::IThreadPolicy> threadPolicy = std::make_unique<utils::ThreadPolicy>();
            return std::make_unique<gossip_protocol::GossipProtocol>(addr, logger, membershipProtocol, threadPolicy);
        }
    };

    void tmp()
    {
            network::Address addr("1.0.0.0:100");
            auto logger = std::make_shared<utils::Log>();
            std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
            std::unique_ptr<gossip_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
            
            membership_protocol::MembershipProtocol membershipProtocol(addr, logger, failureDetectorFactory, goossipProtocolFactory);
            membershipProtocol.start();
            auto members = membershipProtocol.getMembers();
            ASSERT_TRUE(members.empty());
            ASSERT_EQ(membershipProtocol.getMembersNum(), 0);
            membershipProtocol.stop();
    }

    TEST(MembershipProtocolTests, Consructor)
    {
        try
        {
            tmp();
        }
        catch(std::exception const& e)
        {
            std::cerr << e.what();
        }
        catch(...)
        {
            std::cerr << "Exception happened";
        }
    }
}
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../membership_protocol/MembershipProtocol.h"

namespace
{
    class MockIFailureDetector: public membership_protocol::IFailureDetector 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD1(addObserver, void(IObserver* observer));
   };

    class MockIGossipProtocol: public membership_protocol::IGossipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD1(addObserver, void(IObserver* observer));
        MOCK_METHOD1(spreadMembershipUpdate, void(const membership_protocol::MembershipUpdate& membershipUpdate));
    };

    class FailureDetectorFactory: public membership_protocol::IFailureDetectorFactory
    {
        virtual std::unique_ptr<membership_protocol::IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            return std::make_unique<testing::NiceMock<MockIFailureDetector> >();
        }
    };

    class GossipProtocolFactory: public membership_protocol::IGossipProtocolFactory
    {
        virtual std::unique_ptr<membership_protocol::IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            return std::make_unique<testing::NiceMock<MockIGossipProtocol> >();
        }
    };

    TEST(MembershipProtocolTests, Consructor)
    {
        network::Address addr("1.0.0.0:100");
        auto logger = std::make_shared<utils::Log>();
        std::unique_ptr<membership_protocol::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
        std::unique_ptr<membership_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
        
        membership_protocol::MembershipProtocol membershiptProtocol(addr, logger, failureDetectorFactory, goossipProtocolFactory);
    }
}
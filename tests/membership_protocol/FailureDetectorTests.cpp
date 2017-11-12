#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "membership_protocol/failure_detector/FailureDetector.h"

namespace 
{
    class MockIMembershipProtocol : public membership_protocol::IMembershipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(getMembers,  std::vector<membership_protocol::Member>());
        MOCK_METHOD0(getMembersNum, size_t());
        MOCK_METHOD1(addObserver, void(membership_protocol::IMembershipProtocol::IObserver* observer));
    };
    
    class MockIGossipProtocol : public gossip_protocol::IGossipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD1(addObserver, void(IObserver* observer));
        MOCK_METHOD1(spreadMembershipUpdate, void(const membership_protocol::MembershipUpdate& membershipUpdate));
        MOCK_METHOD1(getGossipsForAddress, std::vector<membership_protocol::Gossip>(const network::Address& address));
        MOCK_METHOD2(onNewGossips, void(const network::Address& sourceAddress, const std::vector<membership_protocol::Gossip>& gossips));
    };

    class FailureDetectorTests: public testing::Test, public failure_detector::IFailureDetector::IObserver
    {
    public:
        MockIMembershipProtocol membershipProtocol;
        MockIGossipProtocol gossipProtocol;
        std::unique_ptr<failure_detector::FailureDetector> failureDectector;
        std::vector<network::Address> failedNodes;
        std::vector<network::Address> aliveNodes;
        
        FailureDetectorTests():
            membershipProtocol(),
            gossipProtocol(),
            failureDectector()
        {
            network::Address addr("1.0.0.0:100");
            auto logger = std::make_shared<utils::Log>();
            auto messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            failureDectector = std::make_unique<failure_detector::FailureDetector>(addr, logger, messageDispatcher, &membershipProtocol, &gossipProtocol);    
        }

        void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent)
        {
            switch (failureDetectorEvent.eventType)
            {
                case failure_detector::ALIVE:
                {
                    aliveNodes.push_back(failureDetectorEvent.address);
                    break;
                }
    
                case failure_detector::FAILED:
                {
                    failedNodes.push_back(failureDetectorEvent.address);

                    membership_protocol::MembershipUpdate membershipUpdate(failureDetectorEvent.address, membership_protocol::FAILED);                    
                    failureDectector->onMembershipUpdate(membershipUpdate);
                    break;
                }
    
                default:
                {
                    std::stringstream ss;
                    ss << "unsupported event " << failureDetectorEvent.eventType;
                    throw std::logic_error(ss.str());
                }
            }
        }
    };

    TEST_F(FailureDetectorTests, Constructor)
    {
        failureDectector->start();        
        failureDectector->stop();
    }

    TEST_F(FailureDetectorTests, AddNode)
    {
        using ::testing::_;
        EXPECT_CALL(gossipProtocol, getGossipsForAddress(_)).Times(1);

        failureDectector->addObserver(this);
        failureDectector->start();

        network::Address anotherAddr("2.0.0.0:200");
        membership_protocol::MembershipUpdate membershipUpdate(anotherAddr, membership_protocol::JOINED);
        failureDectector->onMembershipUpdate(membershipUpdate);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        failureDectector->stop();

        ASSERT_EQ(failedNodes.size(), 1);
        ASSERT_EQ(failedNodes[0], anotherAddr);

        ASSERT_EQ(aliveNodes.size(), 0);
    }

    TEST_F(FailureDetectorTests, AddRemoveNode)
    {
        failureDectector->start();

        network::Address anotherAddr("2.0.0.0:200");
        membership_protocol::MembershipUpdate membershipUpdate(anotherAddr, membership_protocol::JOINED);
        failureDectector->onMembershipUpdate(membershipUpdate);

        membershipUpdate = membership_protocol::MembershipUpdate(anotherAddr, membership_protocol::FAILED);
        failureDectector->onMembershipUpdate(membershipUpdate);

        failureDectector->stop();

        ASSERT_EQ(failedNodes.size(), 0);
        ASSERT_EQ(aliveNodes.size(), 0);
    }
}
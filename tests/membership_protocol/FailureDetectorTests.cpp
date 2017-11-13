#include <unordered_set>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "membership_protocol/failure_detector/FailureDetector.h"
#include "membership_protocol/messages/AckMessage.h"

namespace 
{
    using ::testing::AtLeast; 
    using ::testing::_;
    using namespace std::chrono_literals;

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
        network::Address address;
        std::shared_ptr<utils::Log> logger;
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher;

        MockIMembershipProtocol membershipProtocol;
        MockIGossipProtocol gossipProtocol;
        std::unique_ptr<failure_detector::FailureDetector> failureDectector;
        std::vector<network::Address> failedNodes;
        std::unordered_set<network::Address> aliveNodes;
        
        FailureDetectorTests():
            address("1.0.0.0:100"),
            logger(std::make_shared<utils::Log>()),
            messageDispatcher(std::make_shared<utils::MessageDispatcher>(address, logger)),
            membershipProtocol(),
            gossipProtocol(),
            failureDectector()
        {
            messageDispatcher->start();
            failureDectector = std::make_unique<failure_detector::FailureDetector>(address, logger, messageDispatcher, &membershipProtocol, &gossipProtocol);    
        }

        ~FailureDetectorTests()
        {
            messageDispatcher->stop();
        }

        void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent)
        {
            switch (failureDetectorEvent.eventType)
            {
                case failure_detector::ALIVE:
                {
                    aliveNodes.insert(failureDetectorEvent.address);
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

    TEST_F(FailureDetectorTests, AddNodeNoResponse)
    {
        EXPECT_CALL(gossipProtocol, getGossipsForAddress(_)).Times(1);

        failureDectector->addObserver(this);
        failureDectector->start();

        network::Address anotherAddr("2.0.0.0:200");
        membership_protocol::MembershipUpdate membershipUpdate(anotherAddr, membership_protocol::JOINED);
        failureDectector->onMembershipUpdate(membershipUpdate);

        std::this_thread::sleep_for(2s);

        failureDectector->stop();

        ASSERT_EQ(failedNodes.size(), 1);
        ASSERT_EQ(failedNodes[0], anotherAddr);

        ASSERT_EQ(aliveNodes.size(), 0);
    }

    TEST_F(FailureDetectorTests, AddNodeAck)
    {
        EXPECT_CALL(gossipProtocol, getGossipsForAddress(_)).Times(AtLeast(1));

        network::Address anotherAddr("2.0.0.0:200");
        auto anotherMessageDispatcher = std::make_shared<utils::MessageDispatcher>(anotherAddr, logger);
        anotherMessageDispatcher->listen(membership_protocol::PING, [anotherMessageDispatcher, anotherAddr, this](std::unique_ptr<membership_protocol::Message> message){
            anotherMessageDispatcher->sendMessage(std::make_unique<membership_protocol::AckMessage>(anotherAddr, message->getSourceAddress(), std::vector<membership_protocol::Gossip>(), message->getId()), message->getSourceAddress());
        });

        anotherMessageDispatcher->start();

        failureDectector->addObserver(this);
        failureDectector->start();

        membership_protocol::MembershipUpdate membershipUpdate(anotherAddr, membership_protocol::JOINED);
        failureDectector->onMembershipUpdate(membershipUpdate);

        std::this_thread::sleep_for(3s);

        failureDectector->stop();
        anotherMessageDispatcher->stop();

        ASSERT_EQ(failedNodes.size(), 0);

        ASSERT_EQ(aliveNodes.size(), 1);
        ASSERT_EQ(*aliveNodes.begin(), anotherAddr);
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
#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "membership_protocol/gossip_protocol/GossipProtocol.h"

namespace 
{
    class MockIMembershipProtocol : public membership_protocol::IMembershipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(getMembers,  std::vector<membership_protocol::Member>());
        MOCK_METHOD1(addObserver, void(membership_protocol::IMembershipProtocol::IObserver* observer));
   };

   class GossipProtocolTests: public testing::Test, public gossip_protocol::IGossipProtocol::IObserver
   {
    public:
        std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol;
        std::unique_ptr<gossip_protocol::GossipProtocol> gossipProtocol;
        std::vector<network::Address> failedNodes;
        std::vector<network::Address> aliveNodes;
        std::shared_ptr<utils::Log> logger;
        
        GossipProtocolTests():
            membershipProtocol(),
            gossipProtocol(),
            logger(std::make_shared<utils::Log>())
        {
            network::Address addr("1.0.0.0:100");
            // auto logger = std::make_shared<>();
            auto messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            membershipProtocol = std::make_unique<MockIMembershipProtocol>();
            gossipProtocol = std::make_unique<gossip_protocol::GossipProtocol>(addr, logger, messageDispatcher, membershipProtocol.get());    
        }

        void onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate)
        {
            switch (membershipUpdate.updateType)
            {
                case membership_protocol::JOINED:
                {
                    aliveNodes.push_back(membershipUpdate.address);
                    break;
                }

                case membership_protocol::FAILED:
                {
                    failedNodes.push_back(membershipUpdate.address);
                    gossipProtocol->onMembershipUpdate(membershipUpdate);
                    break;
                }

                default:
                {
                    std::stringstream ss;
                    ss << "unsupported event " << membershipUpdate.updateType;
                    throw std::logic_error(ss.str());
                }
            }
        }
   };
    
    /*
    TEST_F(FailureDetectorTests, AddNode)
    {
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
    }*/

    TEST_F(GossipProtocolTests, Constructor)
    {
        gossipProtocol->start();
        gossipProtocol->stop();
    }

    TEST_F(GossipProtocolTests, AddNode)
    {
        network::Address anotherAddr("2.0.0.0:200");
        std::queue<std::unique_ptr<membership_protocol::Message> > queue;
        
        auto anotherMessageDispatcher = std::make_shared<utils::MessageDispatcher>(anotherAddr, logger);
        auto token = anotherMessageDispatcher->listen(membership_protocol::GOSSIP, [&queue](std::unique_ptr<membership_protocol::Message> message){
            queue.push(std::move(message));
        });

        gossipProtocol->addObserver(this);
        gossipProtocol->start();

        membership_protocol::MembershipUpdate membershipUpdate(anotherAddr, membership_protocol::JOINED);
        gossipProtocol->onMembershipUpdate(membershipUpdate);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);

        gossipProtocol->stop();

        ASSERT_EQ(aliveNodes.size(), 0);

        anotherMessageDispatcher->stopListening(membership_protocol::GOSSIP, token);
        ASSERT_TRUE(queue.empty());
    }
}
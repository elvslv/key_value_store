#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "membership_protocol/gossip_protocol/GossipProtocol.h"

namespace 
{
    using ::testing::Return;

    class MockIMembershipProtocol : public membership_protocol::IMembershipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(getMembers,  std::vector<membership_protocol::Member>());
        MOCK_METHOD0(getMembersNum, size_t());
        MOCK_METHOD1(addObserver, void(membership_protocol::IMembershipProtocol::IObserver* observer));
   };

   class GossipProtocolTests: public testing::Test, public gossip_protocol::IGossipProtocol::IObserver
   {
    public:
        std::unique_ptr<MockIMembershipProtocol> membershipProtocol;
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
            auto messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            membershipProtocol = std::make_unique<MockIMembershipProtocol>();
            gossipProtocol = std::make_unique<gossip_protocol::GossipProtocol>(addr, logger, membershipProtocol.get());    
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
                    gossipProtocol->spreadMembershipUpdate(membershipUpdate);
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

    TEST_F(GossipProtocolTests, Constructor)
    {
        gossipProtocol->start();
        gossipProtocol->stop();
    }

    TEST_F(GossipProtocolTests, AddNode)
    {
        gossipProtocol->start();

        EXPECT_CALL(*membershipProtocol.get(), getMembersNum()).WillRepeatedly(Return(3));

        network::Address anotherAddr("2.0.0.0:200");
        auto gossips = gossipProtocol->getGossipsForAddress(anotherAddr);
        ASSERT_TRUE(gossips.empty());

        auto membershipUpdate = membership_protocol::MembershipUpdate(anotherAddr, membership_protocol::JOINED);
        gossipProtocol->spreadMembershipUpdate(membershipUpdate);

        gossips = gossipProtocol->getGossipsForAddress(anotherAddr);
        ASSERT_TRUE(gossips.empty());

        network::Address thirdAddr("3.0.0.0:300");
        gossips = gossipProtocol->getGossipsForAddress(thirdAddr);
        ASSERT_EQ(gossips.size(), 1);

        gossipProtocol->stop();
    }
}
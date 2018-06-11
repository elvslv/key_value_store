#include "../mocks/Mocks.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "membership_protocol/gossip_protocol/GossipProtocol.h"
#include "membership_protocol/messages/Message.h"

namespace
{
using ::testing::Return;

class GossipProtocolTests : public testing::Test, public gossip_protocol::IGossipProtocol::IObserver
{
public:
    mock::MockIMembershipProtocol membershipProtocol;
    std::shared_ptr<utils::IThreadPolicy> threadPolicy;
    std::unique_ptr<gossip_protocol::GossipProtocol> gossipProtocol;
    std::vector<network::Address> failedNodes;
    std::vector<network::Address> aliveNodes;
    std::shared_ptr<utils::Log> logger;

    GossipProtocolTests()
        : membershipProtocol()
        , threadPolicy(new mock::MockIThreadPolicy())
        , gossipProtocol()
        , logger(std::make_shared<utils::Log>())
    {
        network::Address addr("1.0.0.0:100");
        auto messageDispatcher = std::make_shared<utils::MessageDispatcher<membership_protocol::Message>>(addr, logger);
        gossipProtocol = std::make_unique<gossip_protocol::GossipProtocol>(addr, logger, &membershipProtocol, threadPolicy);
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
    EXPECT_CALL(membershipProtocol, getMembersNum())
        .WillOnce(Return(3)) // no gossips
        .WillOnce(Return(3)) // gossips only about given node
        .WillOnce(Return(0)) // no members -> no gossips
        .WillRepeatedly(Return(3));

    network::Address anotherAddr("2.0.0.0:200");
    auto gossips = gossipProtocol->getGossipsForAddress(anotherAddr);
    ASSERT_TRUE(gossips.empty());

    auto membershipUpdate = membership_protocol::MembershipUpdate(anotherAddr, membership_protocol::JOINED);
    gossipProtocol->spreadMembershipUpdate(membershipUpdate);

    gossips = gossipProtocol->getGossipsForAddress(anotherAddr);
    ASSERT_TRUE(gossips.empty());

    network::Address thirdAddr("3.0.0.0:300");
    // membershipProtocol.getMembersNum() will return 0 -> no gossips
    gossips = gossipProtocol->getGossipsForAddress(thirdAddr);
    ASSERT_TRUE(gossips.empty());

    gossips = gossipProtocol->getGossipsForAddress(thirdAddr);
    ASSERT_EQ(gossips.size(), 1);
    auto gossip = gossips[0];
    ASSERT_EQ(gossip.address, membershipUpdate.address);
    ASSERT_EQ(gossip.membershipUpdateType, membershipUpdate.updateType);

    // gossip has been already spread
    gossips = gossipProtocol->getGossipsForAddress(thirdAddr);
    ASSERT_TRUE(gossips.empty());
}
}
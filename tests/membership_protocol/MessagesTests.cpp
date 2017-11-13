#include "gtest/gtest.h"
#include "membership_protocol/messages/PingMessage.h"

namespace
{
    TEST(MessagesTests, PingMessage)
    {
        network::Address from("1.0.0.0:100");
        network::Address to("2.0.0.0:100");

        auto pingMessage = std::make_unique<membership_protocol::PingMessage>(from, to, std::vector<membership_protocol::Gossip>());
        ASSERT_EQ(pingMessage->getMessageType(), membership_protocol::PING);
        ASSERT_EQ(pingMessage->getSourceAddress(), from);
        ASSERT_EQ(pingMessage->getDestinationAddress(), to);
        ASSERT_EQ(pingMessage->getGossips().size(), 0);

        auto id = pingMessage->getId();

        auto networkMessage = pingMessage->serialize();
        auto deserializedMessage = membership_protocol::Message::parseMessage(networkMessage);

        ASSERT_EQ(deserializedMessage->getMessageType(), membership_protocol::PING);
        ASSERT_EQ(deserializedMessage->getSourceAddress(), from);
        ASSERT_EQ(deserializedMessage->getDestinationAddress(), to);
        ASSERT_EQ(deserializedMessage->getId(), id);

        auto deserializedPingMessage = static_cast<membership_protocol::PingMessage*>(deserializedMessage.get());
        ASSERT_EQ(deserializedPingMessage->getGossips().size(), 0);
    }
}
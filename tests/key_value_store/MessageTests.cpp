#include "key_value_store/CreateRequestMessage.h"
#include "gtest/gtest.h"

namespace
{
TEST(MessagesTests, CreateRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";
    std::string value = "value";

    key_value_store::CreateRequestMessage createRequestMessage(sourceAddress, destAddress, key, value);
    ASSERT_EQ(createRequestMessage.getSourceAddress(), sourceAddress);
    ASSERT_EQ(createRequestMessage.getDestinationAddress(), destAddress);
    ASSERT_EQ(createRequestMessage.getKey(), key);
    ASSERT_EQ(createRequestMessage.getValue(), value);

    auto networkMessage = createRequestMessage.serialize();
    auto parsedMessage = key_value_store::Message::parseMessage(networkMessage);
    ASSERT_EQ(parsedMessage->getMessageType(), key_value_store::Message::CREATE_REQUEST);

    std::unique_ptr<key_value_store::CreateRequestMessage> parsedCreateRequestMessage(dynamic_cast<key_value_store::CreateRequestMessage*>(parsedMessage.release()));
    ASSERT_EQ(parsedCreateRequestMessage->getSourceAddress(), sourceAddress);
    ASSERT_EQ(parsedCreateRequestMessage->getDestinationAddress(), destAddress);
    ASSERT_EQ(parsedCreateRequestMessage->getKey(), key);
    ASSERT_EQ(parsedCreateRequestMessage->getValue(), value);
    ASSERT_EQ(parsedCreateRequestMessage->getId(), createRequestMessage.getId());
}
}
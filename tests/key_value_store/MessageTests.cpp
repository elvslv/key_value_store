#include "key_value_store/CreateRequestMessage.h"
#include "key_value_store/DeleteRequestMessage.h"
#include "key_value_store/ReadRequestMessage.h"
#include "gtest/gtest.h"

namespace
{
class KeyValueStoreMessagesTests : public testing::Test
{
public:
    template <typename T>
    void TestMessage(const T& message, const network::Address& sourceAddress, const network::Address& destAddress, const std::string& key, key_value_store::Message::MsgTypes expectedType, std::unique_ptr<T>& parsedCastedMessage)
    {
        ASSERT_EQ(message.getSourceAddress(), sourceAddress);
        ASSERT_EQ(message.getDestinationAddress(), destAddress);
        ASSERT_EQ(message.getKey(), key);

        auto networkMessage = message.serialize();
        auto parsedMessage = key_value_store::Message::parseMessage(networkMessage);
        ASSERT_EQ(parsedMessage->getMessageType(), expectedType);

        parsedCastedMessage.reset(dynamic_cast<T*>(parsedMessage.release()));
        ASSERT_EQ(parsedCastedMessage->getSourceAddress(), sourceAddress);
        ASSERT_EQ(parsedCastedMessage->getDestinationAddress(), destAddress);
        ASSERT_EQ(parsedCastedMessage->getKey(), key);
        ASSERT_EQ(parsedCastedMessage->getId(), message.getId());
    }
};

TEST_F(KeyValueStoreMessagesTests, CreateRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";
    std::string value = "value";

    key_value_store::CreateRequestMessage createRequestMessage(sourceAddress, destAddress, key, value);
    ASSERT_EQ(createRequestMessage.getValue(), value);

    std::unique_ptr<key_value_store::CreateRequestMessage> parsedCastedMessage;
    TestMessage<>(createRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::CREATE_REQUEST, parsedCastedMessage);
    ASSERT_EQ(parsedCastedMessage->getValue(), value);
}

TEST_F(KeyValueStoreMessagesTests, DeleteRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";

    key_value_store::DeleteRequestMessage deleteRequestMessage(sourceAddress, destAddress, key);

    std::unique_ptr<key_value_store::DeleteRequestMessage> parsedCastedMessage;
    TestMessage<>(deleteRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::DELETE_REQUEST, parsedCastedMessage);
}

TEST_F(KeyValueStoreMessagesTests, ReadRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";

    key_value_store::ReadRequestMessage readRequestMessage(sourceAddress, destAddress, key);
    std::unique_ptr<key_value_store::ReadRequestMessage> parsedCastedMessage;
    TestMessage<>(readRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::READ_REQUEST, parsedCastedMessage);
}
}
#include "key_value_store/CreateRequestMessage.h"
#include "key_value_store/CreateResponseMessage.h"
#include "key_value_store/DeleteRequestMessage.h"
#include "key_value_store/DeleteResponseMessage.h"
#include "key_value_store/ReadRequestMessage.h"
#include "key_value_store/ReadResponseMessage.h"
#include "key_value_store/UpdateRequestMessage.h"
#include "key_value_store/UpdateResponseMessage.h"
#include "utils/Utils.h"

#include "gtest/gtest.h"

namespace
{
class KeyValueStoreMessagesTests : public testing::Test
{
public:
    template <typename T>
    void testRequestMessage(const T& message, const network::Address& sourceAddress, const network::Address& destAddress, const std::string& key, key_value_store::Message::MsgTypes expectedType, std::unique_ptr<T>& parsedCastedMessage)
    {
        ASSERT_EQ(message.getSourceAddress(), sourceAddress);
        ASSERT_EQ(message.getDestinationAddress(), destAddress);
        ASSERT_EQ(message.getKey(), key);

        auto networkMessage = message.serialize();
        auto parsedBaseMessage = key_value_store::Message::parseMessage(networkMessage);
        auto parsedMessage = utils::Utils::downcast<T, utils::Message>(std::move(parsedBaseMessage));
        ASSERT_EQ(parsedMessage->getMessageType(), expectedType);

        parsedCastedMessage.reset(dynamic_cast<T*>(parsedMessage.release()));
        ASSERT_EQ(parsedCastedMessage->getSourceAddress(), sourceAddress);
        ASSERT_EQ(parsedCastedMessage->getDestinationAddress(), destAddress);
        ASSERT_EQ(parsedCastedMessage->getKey(), key);
        ASSERT_EQ(parsedCastedMessage->getId(), message.getId());
        ASSERT_EQ(parsedCastedMessage->getTimestamp(), message.getTimestamp());
    }

    template <typename T>
    void testResponseMessage(const T& message, const network::Address& sourceAddress, const network::Address& destAddress, const std::string& originalMessageId, int responseCode, key_value_store::Message::MsgTypes expectedType, std::unique_ptr<T>& parsedCastedMessage)
    {
        ASSERT_EQ(message.getSourceAddress(), sourceAddress);
        ASSERT_EQ(message.getDestinationAddress(), destAddress);
        ASSERT_EQ(message.getOriginalMessageId(), originalMessageId);
        ASSERT_EQ(message.getResponseCode(), responseCode);

        auto networkMessage = message.serialize();
        auto parsedBaseMessage = key_value_store::Message::parseMessage(networkMessage);
        auto parsedMessage = utils::Utils::downcast<T, utils::Message>(std::move(parsedBaseMessage));
        ASSERT_EQ(parsedMessage->getMessageType(), expectedType);

        parsedCastedMessage.reset(dynamic_cast<T*>(parsedMessage.release()));
        ASSERT_EQ(parsedCastedMessage->getSourceAddress(), sourceAddress);
        ASSERT_EQ(parsedCastedMessage->getDestinationAddress(), destAddress);
        ASSERT_EQ(parsedCastedMessage->getOriginalMessageId(), originalMessageId);
        ASSERT_EQ(parsedCastedMessage->getResponseCode(), responseCode);
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
    testRequestMessage<>(createRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::CREATE_REQUEST, parsedCastedMessage);
    ASSERT_EQ(parsedCastedMessage->getValue(), value);
}

TEST_F(KeyValueStoreMessagesTests, DeleteRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";

    key_value_store::DeleteRequestMessage deleteRequestMessage(sourceAddress, destAddress, key);

    std::unique_ptr<key_value_store::DeleteRequestMessage> parsedCastedMessage;
    testRequestMessage<>(deleteRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::DELETE_REQUEST, parsedCastedMessage);
}

TEST_F(KeyValueStoreMessagesTests, ReadRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";

    key_value_store::ReadRequestMessage readRequestMessage(sourceAddress, destAddress, key);
    std::unique_ptr<key_value_store::ReadRequestMessage> parsedCastedMessage;
    testRequestMessage<>(readRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::READ_REQUEST, parsedCastedMessage);
}

TEST_F(KeyValueStoreMessagesTests, UpdateRequestMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string key = "key";
    std::string value = "value";

    key_value_store::UpdateRequestMessage updateRequestMessage(sourceAddress, destAddress, key, value);
    ASSERT_EQ(updateRequestMessage.getValue(), value);

    std::unique_ptr<key_value_store::UpdateRequestMessage> parsedCastedMessage;
    testRequestMessage<>(updateRequestMessage, sourceAddress, destAddress, key, key_value_store::Message::UPDATE_REQUEST, parsedCastedMessage);
    ASSERT_EQ(parsedCastedMessage->getValue(), value);
}

TEST_F(KeyValueStoreMessagesTests, CreateResponseMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string originalMessageId = "originalMessageId";
    int responseCode = 200;

    key_value_store::CreateResponseMessage createResponseMessage(sourceAddress, destAddress, originalMessageId, responseCode);
    std::unique_ptr<key_value_store::CreateResponseMessage> parsedCastedMessage;
    testResponseMessage<>(createResponseMessage, sourceAddress, destAddress, originalMessageId, responseCode, key_value_store::Message::CREATE_RESPONSE, parsedCastedMessage);
}

TEST_F(KeyValueStoreMessagesTests, DeleteResponseMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string originalMessageId = "originalMessageId";
    int responseCode = 200;

    key_value_store::DeleteResponseMessage deleteResponseMessage(sourceAddress, destAddress, originalMessageId, responseCode);
    std::unique_ptr<key_value_store::DeleteResponseMessage> parsedCastedMessage;
    testResponseMessage<>(deleteResponseMessage, sourceAddress, destAddress, originalMessageId, responseCode, key_value_store::Message::DELETE_RESPONSE, parsedCastedMessage);
}

TEST_F(KeyValueStoreMessagesTests, ReadResponseMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string originalMessageId = "originalMessageId";
    std::string value = "value";
    int responseCode = 200;
    unsigned long timestamp = 1;

    key_value_store::ReadResponseMessage readResponseMessage(sourceAddress, destAddress, originalMessageId, responseCode, value, timestamp);
    ASSERT_EQ(readResponseMessage.getValue(), value);
    ASSERT_EQ(readResponseMessage.getTimestamp(), timestamp);

    std::unique_ptr<key_value_store::ReadResponseMessage> parsedCastedMessage;
    testResponseMessage<>(readResponseMessage, sourceAddress, destAddress, originalMessageId, responseCode, key_value_store::Message::READ_RESPONSE, parsedCastedMessage);
    ASSERT_EQ(parsedCastedMessage->getValue(), value);
    ASSERT_EQ(parsedCastedMessage->getTimestamp(), timestamp);
}

TEST_F(KeyValueStoreMessagesTests, UpdateResponseMessage)
{
    network::Address sourceAddress("1.0.0.0:100");
    network::Address destAddress("2.0.0.0:200");
    std::string originalMessageId = "originalMessageId";
    int responseCode = 200;

    key_value_store::UpdateResponseMessage updateResponseMessage(sourceAddress, destAddress, originalMessageId, responseCode);
    std::unique_ptr<key_value_store::UpdateResponseMessage> parsedCastedMessage;
    testResponseMessage<>(updateResponseMessage, sourceAddress, destAddress, originalMessageId, responseCode, key_value_store::Message::UPDATE_RESPONSE, parsedCastedMessage);
}
}
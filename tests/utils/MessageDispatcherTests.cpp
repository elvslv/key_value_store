#include "gtest/gtest.h"
#include "../../utils/MessageDispatcher.h"

namespace
{
    class MessageDispatcherTests: public testing::Test 
    {
    public:
        void callback(std::unique_ptr<membership_protocol::Message> message)
        {
        }
    };

    TEST_F(MessageDispatcherTests, Constructor)
    {
        auto logger = std::make_shared<utils::Log>();
        network::Address address("1.0.0.0:100");
        utils::MessageDispatcher messageDispatcher(address, logger);
    }

    TEST_F(MessageDispatcherTests, GetToken)
    {
        auto logger = std::make_shared<utils::Log>();
        network::Address address("1.0.0.0:100");
        utils::MessageDispatcher messageDispatcher(address, logger);

        // utils::MessageDispatcher::Callback callback = std::bind(&MessageDispatcherTests::callback, this);
        auto token = messageDispatcher.listen(membership_protocol::ACK, [](std::unique_ptr<membership_protocol::Message> message){});

        ASSERT_THROW(messageDispatcher.stopListening(membership_protocol::PING, token), std::logic_error);
        messageDispatcher.stopListening(membership_protocol::ACK, token);
        ASSERT_THROW(messageDispatcher.stopListening(membership_protocol::ACK, token), std::logic_error);        
    }
}
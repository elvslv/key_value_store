#include "gtest/gtest.h"
#include "utils/AsyncQueue.h"
#include "utils/Utils.h"
#include "membership_protocol/messages/PingMessage.h"

namespace 
{
    class AsyncQueueTests: public testing::Test 
    {
    public:
        std::queue<std::unique_ptr<membership_protocol::Message> > messages;

        std::mutex mutex;
        std::condition_variable cv;
        bool ready;

        void callback(std::unique_ptr<membership_protocol::Message> message)
        {
            {
                std::lock_guard<std::mutex> lock(mutex);
                messages.push(std::move(message));
                ready = true; 
            }

            cv.notify_all();
        }

        void waitForMessage()
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this]{return ready;}); 
        }

    protected:
        virtual void setUp()
        {
            ready = false;
        }
    };

    TEST_F(AsyncQueueTests, Constructor)
    {
        utils::AsyncQueue asyncQueue(std::bind(&AsyncQueueTests::callback, this, std::placeholders::_1));
    }
    
    TEST_F(AsyncQueueTests, Push)
    {
        utils::AsyncQueue asyncQueue(std::bind(&AsyncQueueTests::callback, this, std::placeholders::_1));
        asyncQueue.start();

        auto from = network::Address("1.0.0.0:0");
        auto to = network::Address("2.0.0.0:0");
        auto sentMessage = std::make_unique<membership_protocol::PingMessage>(from, to, std::vector<membership_protocol::Gossip>());

        asyncQueue.push(std::move(sentMessage));

        waitForMessage();

        ASSERT_FALSE(messages.empty());
        auto receivedMessage = std::move(messages.front());
        messages.pop();

        ASSERT_TRUE(messages.empty());

        ASSERT_TRUE(utils::Utils::isInstanceOf<membership_protocol::PingMessage>(receivedMessage.get()));

        asyncQueue.stop();
    }
}
    
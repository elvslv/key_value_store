#include <queue>

#include "membership_protocol/messages/AckMessage.h"
#include "membership_protocol/messages/PingMessage.h"
#include "utils/MessageDispatcher.h"
#include "utils/Utils.h"
#include "gtest/gtest.h"

namespace
{
class MessageDispatcherTests : public testing::Test
{
public:
    MessageDispatcherTests()
        : totalCnt()
        , producerAddress("1.0.0.0:100")
        , consumerAddress("2.0.0.0:200")
        , logger(std::make_shared<utils::Log>())
        , queue()
    {
    }

    int totalCnt;
    network::Address producerAddress;
    network::Address consumerAddress;
    std::shared_ptr<utils::Log> logger;
    std::queue<std::unique_ptr<utils::Message>> queue;

    void producer()
    {
        utils::MessageDispatcher messageDispatcher(producerAddress, logger);
        messageDispatcher.start();

        int i = 0;
        while (i < totalCnt)
        {
            int f = std::rand() % 2;
            std::unique_ptr<membership_protocol::Message> message = nullptr;
            if (f == 0)
            {
                message = std::make_unique<membership_protocol::PingMessage>(producerAddress, consumerAddress, std::vector<membership_protocol::Gossip>());
                ++i;
            }
            else
            {
                message = std::make_unique<membership_protocol::AckMessage>(producerAddress, consumerAddress, std::vector<membership_protocol::Gossip>(), "str");
            }

            messageDispatcher.sendMessage(std::move(message), consumerAddress);

            int sleepTimeout = std::rand() % 50;
            std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeout));
        }

        messageDispatcher.stop();
    }

    void consumer()
    {
        utils::MessageDispatcher messageDispatcher(consumerAddress, logger);
        std::mutex mutex;
        std::condition_variable cv;
        volatile int receivedCnt = 0;

        auto token = messageDispatcher.listen(utils::Message::getTypeName<membership_protocol::PingMessage>(), [this, &mutex, &receivedCnt, &cv](std::unique_ptr<utils::Message> message) {
            queue.push(std::move(message));

            std::lock_guard<std::mutex> lock(mutex);
            ++receivedCnt;

            cv.notify_all();
        });

        messageDispatcher.start();
        {
            std::unique_lock<std::mutex> lock(mutex);
            cv.wait(lock, [this, &receivedCnt] { return receivedCnt == totalCnt; });
        }

        messageDispatcher.stopListening(token);
        messageDispatcher.stop();
    }
};

TEST_F(MessageDispatcherTests, Constructor)
{
    utils::MessageDispatcher messageDispatcher(producerAddress, logger);
}

TEST_F(MessageDispatcherTests, GetToken)
{
    utils::MessageDispatcher messageDispatcher(producerAddress, logger);

    auto token = messageDispatcher.listen(utils::Message::getTypeName<membership_protocol::AckMessage>(), [](std::unique_ptr<utils::Message> message) {});

    messageDispatcher.stopListening(token);
    ASSERT_THROW(messageDispatcher.stopListening(token), std::logic_error);
}

TEST_F(MessageDispatcherTests, Listen)
{
    totalCnt = 10;

    std::thread consumer_thread(&MessageDispatcherTests::consumer, this);
    std::thread producer_thread(&MessageDispatcherTests::producer, this);

    producer_thread.join();
    consumer_thread.join();

    ASSERT_EQ(queue.size(), totalCnt);
    while (!queue.empty())
    {
        auto message = std::move(queue.front());
        queue.pop();

        ASSERT_TRUE(utils::Utils::isInstanceOf<membership_protocol::PingMessage>(message.get()));
    }
}
}
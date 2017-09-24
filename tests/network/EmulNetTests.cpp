#include <unordered_map>
#include <thread>

#include "gtest/gtest.h"
#include "../../network/EmulNet.h"
#include "../../network/Message.h"

namespace 
{
    class EmulNetTests: public testing::Test 
    {
    public:
        EmulNetTests():
            sentIds(),
            receivedIds(),
            receivedCnt(0),
            totalCnt(),
            producerAddress("1.0.0.0:100"),
            consumerAddress("2.0.0.0:200")
        {
        }

        std::unordered_map<int, int> sentIds;
        std::unordered_map<int, int> receivedIds;        
        int receivedCnt;
        int totalCnt;
        network::Address producerAddress;
        network::Address consumerAddress;

        void producer()
        {
            for (int i = 0; i < totalCnt; ++i)
            {
                int num = rand();
                if (sentIds.find(num) == sentIds.end())
                {
                    sentIds[num] = 0;
                }

                ++sentIds[num];    

                auto emulNet = network::EmulNet::instance();
                network::Message message(std::to_string(num));
                
                emulNet->send(producerAddress, consumerAddress, message);

                int sleepTimeout = std::rand() % 50;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeout));
            }
        }

        void consumer()
        {
            while (receivedCnt < totalCnt)
            {
                auto emulNet = network::EmulNet::instance();
                auto message = emulNet->receive(consumerAddress);

                if (!message.empty())
                {
                    auto num_str = std::string(message.content.get(), message.size);
                    int num = std::stoi(num_str);
                    if (receivedIds.find(num) == receivedIds.end())
                    {
                        receivedIds[num] = 0;
                    }
                    ++receivedIds[num];
                    ++receivedCnt;
                }

                int sleepTimeout = std::rand() % 50;
                std::this_thread::sleep_for(std::chrono::milliseconds(sleepTimeout));
            }
        }
    };

    TEST_F(EmulNetTests, GetInstance)
    {
        auto emulNet = network::EmulNet::instance();
    }

    TEST_F(EmulNetTests, ShareInstances)
    {
        auto emulNet1 = network::EmulNet::instance();
        auto emulNet2 = network::EmulNet::instance();
    }

    TEST_F(EmulNetTests, SendAndReceive)
    {
        network::Message sentMessage("Hello world!");
        ASSERT_FALSE(sentMessage.empty());
        
        auto emulNet = network::EmulNet::instance();
        
        network::Address address("1.0.0.0:101");
        network::Address srcAddress("2.0.0.0:123");

        emulNet->send(srcAddress, address, sentMessage);

        auto receivedMessage = emulNet->receive(srcAddress);
        ASSERT_TRUE(receivedMessage.empty());

        receivedMessage = emulNet->receive(address);
        ASSERT_FALSE(receivedMessage.empty());
        
        ASSERT_TRUE(sentMessage == receivedMessage);

        receivedMessage = emulNet->receive(address);
        ASSERT_TRUE(receivedMessage.empty());        
    }

    TEST_F(EmulNetTests, TwoThreads)
    {
        totalCnt = 10;

        std::thread producer_thread(&EmulNetTests::producer, this);
        std::thread consumer_thread(&EmulNetTests::consumer, this);
        
        consumer_thread.join();
        producer_thread.join();

        ASSERT_TRUE(totalCnt == receivedCnt);
        ASSERT_TRUE(sentIds.size() == receivedIds.size());
        for (auto it = sentIds.begin(); it != sentIds.end(); ++it)
        {
            ASSERT_TRUE(sentIds[it->first] == receivedIds[it->first]);
        }
    }
}
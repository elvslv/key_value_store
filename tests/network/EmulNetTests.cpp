#include "gtest/gtest.h"
#include "../../network/EmulNet.h"
#include "../../network/Message.h"

namespace 
{
    TEST(EmulNetTests, GetInstance)
    {
        auto emulNet = network::EmulNet::instance();
    }

    TEST(EmulNetTests, ShareInstances)
    {
        auto emulNet1 = network::EmulNet::instance();
        auto emulNet2 = network::EmulNet::instance();
    }

    TEST(EmulNetTests, SendAndReceive)
    {
        network::Message sentMessage("Hello world!");
        ASSERT_FALSE(sentMessage.empty());
        
        auto emulNet = network::EmulNet::instance();
        
        network::Address address("1.0.0.0:101");
        network::Address srcAddress("2.0.0.0:123");

        emulNet->send(srcAddress, address, sentMessage);

        auto receivedMessage = emulNet->receive(address);
        ASSERT_FALSE(receivedMessage.empty());
        
        ASSERT_TRUE(sentMessage == receivedMessage);
    }
}
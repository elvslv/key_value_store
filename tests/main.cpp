#include "gtest/gtest.h"
#include "Log.h"

namespace
{
    class LogTests: public testing::Test 
    {
    protected:
        LogTests(): 
            logger(),
            address({{0, 0, 0, 0}}, 0)
        {
        }

        virtual void SetUp() 
        {
            address.address = {{1, 0, 0, 0}};
            address.port = 125;
        }

        utils::Log logger;
        network::Address address;
    };

    TEST_F(LogTests, SimpleLog)
    {
        logger.log(address, "123");
    }

    TEST_F(LogTests, AnotherLog)
    {
        double d = 7.0;
        logger.log(address, "123", d, 345);
    }
}

int main(int argc, char **argv) 
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
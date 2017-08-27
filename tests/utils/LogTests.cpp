#include "gtest/gtest.h"
#include "Log.h"

namespace
{
    TEST(LogTests, SimpleLog)
    {
        utils::Log logger;
        logger.log("123");
    }

    TEST(LogTests, AnotherLog)
    {
        utils::Log logger;
        double d = 7.0;
        logger.log("123", d, 345);
    }
}
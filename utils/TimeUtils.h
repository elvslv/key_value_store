#pragma once

#include <ctime>
#include <string>

namespace utils
{
    struct Time
    {
        static std::string getCurrentTime();
        static long getTimestamp();
    };
}
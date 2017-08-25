#include <ctime>
#include "TimeUtils.h"

namespace utils
{
    std::string Time::getCurrentTime()
    {
        std::time_t result = std::time(nullptr);
        return std::ctime(&result);
    }
}
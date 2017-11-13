#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "TimeUtils.h"

namespace utils
{
    std::string Time::getCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto now_time = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    long Time::getTimestamp()
    {
        std::time_t result = std::time(nullptr);
        return static_cast<long>(result);
    }
}
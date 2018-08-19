#include "TimeUtils.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

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

unsigned long Time::getTimestamp()
{
    std::time_t result = std::time(nullptr);
    return static_cast<unsigned long>(result);
}
}
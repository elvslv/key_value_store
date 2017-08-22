#include <ctime>

namespace utils
{
    struct Time
    {
        std::string Time::getCurrentTime()
        {
            std::time_t result = std::time(nullptr);
            return std::ctime(&result);
        }
    }
}
#include "ThreadPolicy.h"
#include <chrono>
#include <thread>

namespace utils
{
    void ThreadPolicy::sleepMilliseconds(unsigned int milliseconds) const
    {
        auto sleep_duration = std::chrono::milliseconds(milliseconds);
        std::this_thread::sleep_for(sleep_duration);
    }
}
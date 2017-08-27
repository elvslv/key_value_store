#include <iostream>
#include <sstream>
#include "Log.h"
#include "TimeUtils.h"

namespace utils
{
    Log::Log():
        mutex(),
        output(DBG_LOG)
    {
    }

    void Log::log_(const std::string& str)
    {
        std::stringstream ss;
        ss << "[" << utils::Time::getCurrentTime() << "] " << str << std::endl;

        std::lock_guard<std::mutex> lock(mutex);
        output << ss.str();
        output.flush();
    }
}

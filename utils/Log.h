#pragma once

#include <fstream>
#include <mutex>
#include <sstream>
#include "TimeUtils.h"

namespace utils
{
    class Log
    {
    public:
        Log();

        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            std::stringstream ss;
            construct_log_line(ss, current, next...);
            
            log_(ss.str());
        }
    private:
        template <typename T >
        void construct_log_line(std::stringstream& ss, T only)
        {
            ss << only << std::endl;
        }

        template <typename T, typename ... args >
        void construct_log_line(std::stringstream& ss, T current, args... next )
        {
            ss << current << ' ';
            construct_log_line(ss, next... );
        }

        void log_(const std::string& str);

        const std::string DBG_LOG = "dbg.log";

        std::mutex mutex;
        std::ofstream output;
    };
}

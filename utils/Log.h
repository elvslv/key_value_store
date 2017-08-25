#pragma once

#include <fstream>
#include <mutex>
#include <sstream>
#include "../network/Address.h"

namespace utils
{
    class Log
    {
    public:
        Log();

        void logNodeAdd(const network::Address&, const network::Address&);
        void logNodeRemove(const network::Address&, const network::Address&);

        template <typename T, typename ... args >
        void log(const network::Address& node, T current, args... next )
        {
            std::stringstream ss;
            construct_log_line(ss, current, next...);
            
            log_(node, ss.str());
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
            log_(ss, next... );
        }

        void log_(const network::Address& node, const std::string& str);

        const std::string DBG_LOG = "dbg.log";

        std::mutex mutex;
        std::ofstream output;
    };
}

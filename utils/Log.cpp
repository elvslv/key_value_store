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

    void Log::log_(const network::Address& node, const std::string& str)
    {
        std::stringstream ss;
        ss << node.toString() << " [" << utils::Time::getCurrentTime() << "] " << str << std::endl;

        std::lock_guard<std::mutex> lock(mutex);
        output << ss.str();
        output.flush();
    }

    void Log::logNodeAdd(const network::Address& thisNode, const network::Address& addedAddr) 
    {
        std::stringstream ss;
        ss << "Node " << addedAddr.toString() << " joined at time " << utils::Time::getCurrentTime();
        log(thisNode, ss.str());
    }

    void Log::logNodeRemove(const network::Address& thisNode, const network::Address& removedAddr) 
    {
        std::stringstream ss;
        ss << "Node " << removedAddr.toString() << " joined at time " << utils::Time::getCurrentTime();
        log(thisNode, ss.str());
    }
}

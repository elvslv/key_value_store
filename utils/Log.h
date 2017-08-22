#pragma once

#include "../network/Address.h"

/*
 * Macros
 */
// number of writes after which to flush file
#define MAXWRITES 1
#define MAGIC_NUMBER "CS425"
#define DBG_LOG "dbg.log"
#define STATS_LOG "stats.log"

namespace utils
{
    class Log
    {
    private:
        // Params *par;
        // bool firstTime;
        std::mutex mutex;
    public:
        // Log(Params *p);
        // Log(const Log &anotherLog);
        // Log& operator = (const Log &anotherLog);
        virtual ~Log();
        void LOG(const network::Address&, const char * str, ...);
        void logNodeAdd(const network::Address&, const network::Address&);
        void logNodeRemove(const network::Address&, const network::Address&);
    };
}

#pragma once

namespace utils
{
    class NoOpLock
    {
    public:
        void lock(){}
        void unlock(){}
    };
}
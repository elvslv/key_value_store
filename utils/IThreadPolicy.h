#pragma once
#include <cstdlib>

namespace utils
{
    class IThreadPolicy
    {
    public:
        virtual ~IThreadPolicy(){};
        virtual void sleepMilliseconds(unsigned int milliseconds) const = 0;
    };
}
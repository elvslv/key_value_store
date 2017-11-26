#include "IThreadPolicy.h"

namespace utils
{
    struct ThreadPolicy: IThreadPolicy
    {
        virtual void sleepMilliseconds(unsigned int milliseconds) const;
    };
}
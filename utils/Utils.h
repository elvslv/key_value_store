#pragma once
#include <string>

namespace utils
{
    class Utils
    {
    public:
        static std::string getRandomString(size_t length);

        template<typename CheckType, typename InstanceType>
        static bool isInstanceOf(InstanceType& Instance) 
        {
            return (dynamic_cast<CheckType *>(&Instance) != NULL);
        }

        template<typename CheckType, typename InstanceType>
        static bool isInstanceOf(InstanceType* Instance) 
        {
            return (dynamic_cast<CheckType *>(Instance) != NULL);
        }
    };
}
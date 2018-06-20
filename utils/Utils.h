#pragma once
#include <string>

namespace utils
{
class Utils
{
public:
    static std::string getRandomString(size_t length);

    template <typename CheckType, typename InstanceType>
    static bool isInstanceOf(InstanceType& Instance)
    {
        return (dynamic_cast<CheckType*>(&Instance) != NULL);
    }

    template <typename CheckType, typename InstanceType>
    static bool isInstanceOf(InstanceType* Instance)
    {
        return (dynamic_cast<CheckType*>(Instance) != NULL);
    }

    template <typename TargetType, typename InstanceType>
    static std::unique_ptr<TargetType> downcast(std::unique_ptr<InstanceType> instance)
    {
        TargetType* tmp = dynamic_cast<TargetType*>(instance.get());
        assert(tmp != nullptr);
        std::unique_ptr<TargetType> result;
        instance.release();
        result.reset(tmp);

        return result;
    }
};
}
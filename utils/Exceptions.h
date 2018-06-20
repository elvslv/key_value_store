#pragma once
#include <list>
#include <sstream>
#include <stdexcept>

namespace utils
{
class ComplexException : public std::exception
{
public:
    ComplexException(){};
    void addException(std::exception_ptr exc)
    {
        exceptions.push_back(exc);
    }

    virtual const char* what() const _NOEXCEPT
    {
        std::stringstream ss;
        for (auto exception : exceptions)
        {
            try
            {
                std::rethrow_exception(exception);
            }
            catch (const std::exception& ex)
            {
                ss << ex.what() << std::endl;
            }
        }

        auto str = ss.str();
        char* result = new char[str.size()];
        strcpy(result, str.c_str());
        return result;
    }

private:
    std::list<std::exception_ptr> exceptions;
};

class NotImplementedException : public std::logic_error
{
public:
    NotImplementedException()
        : std::logic_error("Function not yet implemented"){};
};
}

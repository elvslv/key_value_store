#pragma once
#include <list>
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

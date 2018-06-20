#pragma once

#include <exception>
#include <string>

namespace key_value_store
{

class NotFoundException : public std::exception
{
public:
    NotFoundException(const std::string& key);

private:
    std::string key;
};

class ConflictDetected : public std::exception
{
public:
    ConflictDetected(const std::string& key);

private:
    std::string key;
};
}
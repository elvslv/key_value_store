#pragma once

#include <string>
#include <vector>

namespace key_value_store
{
struct Record
{
    Record()
        : value()
    {
    }

    Record(const std::string& value, unsigned long timestamp)
        : value(value)
        , timestamp(timestamp)
    {
    }
    std::string value;
    unsigned long timestamp;
};

class IStorage
{
public:
    virtual ~IStorage() {}

    virtual Record get(const std::string& key) = 0;
    virtual void insert(const std::string& key, const Record& value) = 0;
    virtual void update(const std::string& key, const Record& value) = 0;
    virtual void remove(const std::string& key) = 0;

    virtual std::vector<std::pair<std::string, Record>> getRecords() = 0;
};
}
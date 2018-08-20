#pragma once

#include "key_value_store/IStorage.h"
#include <map>
#include <thread>

namespace key_value_store
{
class Storage : public IStorage
{
public:
    virtual ~Storage() {}

    virtual Record get(const std::string& key);
    virtual void insert(const std::string& key, const Record& value);
    virtual void update(const std::string& key, const Record& value);
    virtual void remove(const std::string& key, unsigned long timestamp);

    virtual std::vector<std::pair<std::string, Record>> getRecords();

private:
    std::map<std::string, Record> map;
    std::mutex mutex;
};
}
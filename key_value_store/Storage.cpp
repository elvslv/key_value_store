#include "key_value_store/Storage.h"
#include "key_value_store/Exceptions.h"

namespace key_value_store
{
Record Storage::get(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = map.find(key);
    if (it == map.end() || it->second.removed)
    {
        throw NotFoundException(key);
    }

    return it->second;
}

void Storage::insert(const std::string& key, const Record& value)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = map.find(key);
    if (it != map.end() && !it->second.removed)
    {
        throw ConflictDetected(key);
    }

    if (it == map.end() || value.timestamp > map[key].timestamp)
    {
        map[key] = value;
    }
}

void Storage::update(const std::string& key, const Record& value)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = map.find(key);
    if (it == map.end() || it->second.removed)
    {
        throw NotFoundException(key);
    }

    if (value.timestamp > map[key].timestamp)
    {
        map[key] = value;
    }
}

void Storage::remove(const std::string& key, unsigned long timestamp)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto it = map.find(key);
    if (it == map.end() || it->second.removed)
    {
        throw NotFoundException(key);
    }

    map[key].removed = true;
    map[key].timestamp = timestamp;
}

std::vector<std::pair<std::string, Record>> Storage::getRecords()
{
    std::map<std::string, Record> copy;
    {
        std::lock_guard<std::mutex> lock(mutex);
        copy = map;
    }

    std::vector<std::pair<std::string, Record>> result;
    result.reserve(copy.size());
    for (auto it = copy.begin(); it != copy.end(); ++it)
    {
        result.push_back(std::make_pair(it->first, it->second));
    }

    return result;
}
}
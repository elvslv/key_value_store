#include "Partitioner.h"
#include <functional>

namespace key_value_store
{
Partitioner::Partitioner(size_t ringSize)
    : ringSize(ringSize)
{
}

std::vector<network::Address> Partitioner::getTargetNodes(const std::string& key, std::vector<membership_protocol::Member> members)
{
    std::sort(members.begin(), members.end(), [this](const membership_protocol::Member& member1, const membership_protocol::Member& member2) {
        return member1.hash(ringSize) < member2.hash(ringSize);
    });

    std::vector<network::Address> result;
    int h = std::hash<std::string>{}(key) % ringSize;
    for (int i = 0; i < members.size(); ++i)
    {
        int index = -1;
        if (members[i].hash(ringSize) >= h)
        {
            index = i;
        }
        else if (i == members.size() - 1)
        {
            index = 0;
        }

        if (index == -1)
        {
            continue;
        }

        result.push_back(members[index].address);
        if (members.size() > 1)
        {
            result.push_back(members[(index + 1) % members.size()].address);
        }
        if (members.size() > 2)
        {
            result.push_back(members[(index + 2) % members.size()].address);
        }

        break;
    }

    return result;
}
}
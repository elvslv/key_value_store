#pragma once

#include "IPartitioner.h"
#include <vector>

namespace key_value_store
{
class Partitioner : public IPartitioner
{
public:
    Partitioner(size_t ringSize);
    virtual ~Partitioner() {}
    virtual std::vector<network::Address> getTargetNodes(const std::string& key, std::vector<membership_protocol::Member> members);

private:
    size_t ringSize;
};
}
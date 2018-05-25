#pragma once

#include "membership_protocol/Member.h"
#include "network/Address.h"
#include <vector>

namespace key_value_store
{
class IPartitioner
{
public:
    virtual ~IPartitioner() {}
    virtual std::vector<network::Address> getTargetNodes(const std::string& key, std::vector<membership_protocol::Member> members) = 0;
};
}
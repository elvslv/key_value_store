#pragma once

#include <list>
#include "network/Address.h"
#include "membership_protocol/Member.h"

namespace key_value_store
{
    class IPartitioner
    {
    public:
        virtual ~IPartitioner(){}
        virtual std::list<network::Address> getTargetNodes(const std::string& key, const std::vector<membership_protocol::Member>& members) = 0;
    };
}
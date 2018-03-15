#pragma once

#include <list>
#include "IPartitioner.h"

namespace key_value_store
{
    class Partitioner: IPartitioner
    {
    public:
        Partitioner(size_t ringSize);
        virtual ~Partitioner(){}
        virtual std::list<network::Address> getTargetNodes(const std::string& key, const std::vector<membership_protocol::Member>& members);
    private:
        size_t ringSize;
    };
}
#pragma once

#include <vector>
#include "Node.h"
#include "../network/Network.h"

namespace membership_protocol
{
    class MembershipProtocol
    {
    public:
        MembershipProtocol(const network::Address& addr);
        const std::vector<Node>& getMembers() const;
    private:
        Node node;
        std::vector<Node> members; 
        network::Network network;
    };
}
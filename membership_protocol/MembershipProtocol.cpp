#include "MembershipProtocol.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr): 
        node(addr),
        members(),
        network(addr)
    {
    }

    const std::vector<Node>& MembershipProtocol::getMembers() const
    {
        return members;
    }
}
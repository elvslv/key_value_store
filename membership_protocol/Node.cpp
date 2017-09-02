#include <functional>
#include "Constants.h"
#include "Node.h"

namespace membership_protocol
{
    Node::Node(const network::Address& addr) :
        address(addr),
        timestamp(0)
    {
    }

    const network::Address& Node::getAddress() const
    {
        return address;
    }

    size_t Node::getHashCode() const
    {
        return hash_fn(address.toString()) % RING_SIZE;
    }

    long Node::getTimestamp() const 
    {
        return timestamp;
    }

    void Node::setTimestamp(long val)
    {
        timestamp = val;
    }
}
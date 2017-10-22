#include "Gossip.h"

namespace membership_protocol
{
    Gossip::Gossip(const network::Address& address, GossipType gossipType, const std::string& id):
        address(address),
        gossipType(gossipType),
        id(id)
    {
    }

    void Gossip::serializeTo(gen::Gossip* gossip) const
    {
        auto addressSerialized = address.serialize();
        
        gossip->set_allocated_address(addressSerialized.release());
        gossip->set_event(getProtobufEventsType());
        gossip->set_id(id);
    }

    gen::GossipEventTypes Gossip::getProtobufEventsType() const
    {
        switch (gossipType)
        {
            case JOINED:
                return gen::JOINED;
            case FAILED:
                return gen::FAILED;
        }

        throw std::logic_error("Unexpected event type");
    }
}
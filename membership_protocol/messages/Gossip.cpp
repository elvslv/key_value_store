#include "Gossip.h"

namespace membership_protocol
{
    Gossip::Gossip(const network::Address& address, MembershipUpdateType membershipUpdateType, const std::string& id):
        address(address),
        membershipUpdateType(membershipUpdateType),
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
        switch (membershipUpdateType)
        {
            case JOINED:
                return gen::JOINED;
            case FAILED:
                return gen::FAILED;
        }

        throw std::logic_error("Unexpected event type");
    }
}
#include "Events.h"

namespace membership_protocol
{
    Event::Event(EventTypes type, const network::Address& addr) :
        eventType(type),
        nodeAddress(addr)
    {
    }

    EventTypes Event::getEventType() const
    {
        return eventType;
    }

    const network::Address& Event::getNodeAddress() const
    {
        return nodeAddress;
    }

    gen::EventTypes Event::getProtobufEventsType(membership_protocol::EventTypes eventType)
    {
        switch (eventType)
        {
            case membership_protocol::JOINED:
                return gen::JOINED;

            case membership_protocol::FAILED:
                return gen::FAILED;
        }
    }

    std::unique_ptr<gen::Event> Event::serialize() const
    {
        gen::Event* event = new gen::Event;
        auto address = nodeAddress.serialize();
        
        event->set_allocated_address(address.release());
        event->set_event(getProtobufEventsType(eventType));

        return std::unique_ptr<gen::Event>(event);
    }
}
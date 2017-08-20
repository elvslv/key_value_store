#include "Events.h"

namespace membership_protocol
{
    Event::Event(EventTypes type, const network::Address& addr, long t) :
        eventType(type),
        nodeAddress(addr),
        timestamp(t)
    {
    }


    std::unique_ptr<gen::Event> Event::serialize()
    {
        gen::Event* event = new gen::Event;
        auto address = nodeAddress.serialize();
        
        event->set_allocated_address(address.release());
        event->set_event(getProtobufEventsType(eventType));

        return std::unique_ptr<gen::Event>(event);
    }

    gen::EventTypes Event::getProtobufEventsType(EventTypes eventType)
    {
        switch (eventType)
        {
            case JOINED:
                return gen::JOINED;

            case FAILED:
                return gen::FAILED;
        }
    }

    EventTypes Event::getEventType() const
    {
        return eventType;
    }

    const network::Address& Event::getNodeAddress() const
    {
        return nodeAddress;
    }

    long Event::getTimestamp()
    {
        return timestamp;
    }
}
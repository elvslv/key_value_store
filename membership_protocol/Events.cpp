#include "Events.h"
#include "../proto/ProtobufFactory.h"

namespace membership_protocol
{
    Event::Event(EventTypes type, const network::Address& addr, long t) :
        eventType(type),
        nodeAddress(addr),
        timestamp(t)
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

    long Event::getTimestamp()
    {
        return timestamp;
    }
}
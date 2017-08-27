#pragma once
#include <memory>
#include "../network/Address.h"
#include "../proto/Message.pb.h"

namespace membership_protocol
{
    enum EventTypes
    {
        JOINED,
        FAILED,
    };

    class Event
    {
    public:
        Event(EventTypes type, const network::Address& addr, long t);

        std::unique_ptr<gen::Event> serialize();
        EventTypes getEventType() const;
        const network::Address& getNodeAddress() const;
        long getTimestamp();

    private:
        static gen::EventTypes getProtobufEventsType(EventTypes eventType);

        EventTypes eventType;
        network::Address nodeAddress;
        long timestamp;
    };
}
#include <sstream>
#include "PingMessageBase.h"

namespace membership_protocol
{
    PingMessageBase::PingMessageBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Event>& ev): 
        Message(msgType, from, to),
        events(ev)
    {
    }

    gen::Message PingMessageBase::serializeToProtobuf()
    {
        auto message = Message::serializeToProtobuf();
        auto list = message.mutable_events();
        list->Reserve(events.size());

        for (auto it = events.begin(); it != events.end(); ++it)
        {
            auto event = it->serialize();
            list->AddAllocated(event.release());
        }

        return message;
    }

    std::string PingMessageBase::toString()
    {
        auto result = Message::toString();
        std::stringstream ss;

        ss << result << std::endl;
        for (std::vector<Event>::const_iterator it = events.begin(); it != events.end(); ++it)
        {
            ss << "event: " << it->getEventType() << " " << it->getNodeAddress().toString() << std::endl; 
        }

        return ss.str();
    }

    const std::vector<Event>& PingMessageBase::getEvents()
    {
        return events;
    }
}
#pragma once

#include "Message.h"
#include "Events.h"

namespace membership_protocol
{
    class PingMessageBase : public Message
    {
    public:
        virtual std::string toString();
        const std::vector<Event>& getEvents();

    protected:
        PingMessageBase(MsgTypes msgType, const network::Address& from, const network::Address& to, const std::vector<Event>& ev);
        virtual gen::Message serializeToProtobuf();
        virtual gen::MessageType getProtobufMessageType() = 0;

    private:
        std::vector<Event> events;
    };
}
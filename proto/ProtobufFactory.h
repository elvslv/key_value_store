#pragma once
#include <memory>
#include "Address.pb.h"
#include "Message.pb.h"
#include "../network/Address.h"
#include "../membership_protocol/Events.h"
#include "../membership_protocol/Message.h"

namespace protobuf
{
    class ProtobufFactory
    {
    public:
        template<typename T, typename U>
        static U serialize(const T& obj);

        template<typename T, typename U>
        static U deserialize(const T& obj);
    private:
        static gen::EventTypes getProtobufEventsType(membership_protocol::EventTypes eventType);
    };

    template<>
    std::unique_ptr<gen::Address> ProtobufFactory::serialize(const network::Address& obj)
    {
        gen::Address* addr = new gen::Address;
        for (auto it = obj.address.begin(); it != obj.address.end(); ++it)
        {
            addr->add_addr(*it);
        }

        addr->set_port(obj.port);

        return std::unique_ptr<gen::Address>(addr);
    }

    template<>    
    std::unique_ptr<gen::Event> ProtobufFactory::serialize(const membership_protocol::Event& obj)
    {
        gen::Event* event = new gen::Event;
        auto address = protobuf::ProtobufFactory::serialize<network::Address, std::unique_ptr<gen::Address> >(obj.getNodeAddress());
        
        event->set_allocated_address(address.release());
        event->set_event(getProtobufEventsType(obj.getEventType()));

        return std::unique_ptr<gen::Event>(event);
    }

    template<>    
    network::Address ProtobufFactory::deserialize(const gen::Address& obj)
    {
        std::array<unsigned char, 4> address;
        assert(obj.addr_size() == 4);
        for (int i = 0; i < 4; ++i)
        {
            address[i] = obj.addr(i);
        }

        return network::Address(address, obj.port());
    }
}

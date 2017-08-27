#include <iostream>
#include <memory>
#include <sstream>

#include "Message.h"
#include "JoinReqMessage.h"
#include "JoinRepMessage.h"
#include "../proto/Message.pb.h"
#include "../proto/ProtobufFactory.h"
#include "PingMessage.h"
#include "AckMessage.h"
#include "Events.h"
#include "Node.h"

namespace membership_protocol
{
    Message::Message(MsgTypes msgType, const network::Address& srcAddress, const network::Address& destAddress) :
        from(srcAddress),
        to(destAddress),
        messageType(msgType)
    {
    }

    Message::~Message(){}


    MsgTypes Message::getMessageType() const
    {
        return messageType;
    }

    std::unique_ptr<Message> Message::parseMessage(const network::Message& networkMessage)
    {
        gen::Message message;
        message.ParseFromArray(networkMessage.content.get(), networkMessage.size);

       auto srcAddress = protobuf::ProtobufFactory::deserialize<gen::Address, network::Address>(message.sourceaddress());
       auto destAddress = protobuf::ProtobufFactory::deserialize<gen::Address, network::Address>(message.destinationaddress());

        switch (message.messagetype())
        {
            case gen::JOINREQ:
            {
                return std::make_unique<JoinReqMessage>(srcAddress, destAddress);
            }

            case gen::JOINREP:
            {
                int members_num = message.members_size();
                std::vector<Node> members;
                members.reserve(members_num);

                for (int i = 0; i < members_num; ++i)
                {
                    gen::Node genMember = message.members(i);
                    gen::Address addr = genMember.address();
                    members.emplace_back(network::Address(addr));
                }

                return std::make_unique<JoinRepMessage>(srcAddress, destAddress, members);
            }

            case gen::PING:
            {
                int events_num = message.events_size();
                std::vector<Event> events;

                for (int i = 0; i < events_num; ++i)
                {
                    gen::Event event = message.events(i);
                    EventTypes type;
                    switch (event.event())
                    {
                        case gen::JOINED:
                            type = JOINED;
                            break;

                        case gen::FAILED:
                            type = FAILED;
                            break;

                        default:
                            assert(false);
                    }

                    network::Address address(event.address());
                    events.emplace_back(type, address, 0);
                }

                return std::make_unique<PingMessage>(srcAddress, destAddress, events);
            }

            case gen::ACK:
            {
                return std::make_unique<AckMessage>(srcAddress, destAddress);
            }

            case gen::PING_REQ:
            {
                throw std::logic_error("not impelemnted");
            }
        }

        throw std::logic_error("not impelemnted");
    }

    const network::Address& Message::getSourceAddress()
    {
        return from;
    }

    const network::Address& Message::getDestinationAddress()
    {
        return to;
    }

    network::Message Message::serialize()
    {        
        gen::Message message = serializeToProtobuf();

        unsigned int size = message.ByteSize();
        char* data = new char[size];
        message.SerializeToArray(data, size);

        return network::Message(data, size);
    }

    gen::Message Message::serializeToProtobuf()
    {
        auto srcAddress = from.serialize();
        auto destAddress = to.serialize();

        gen::Message message;
        message.set_messagetype(getProtobufMessageType());
        message.set_allocated_sourceaddress(srcAddress.release());
        message.set_allocated_destinationaddress(destAddress.release());

        return message;
    }

    std::string Message::toString()
    {
        std::stringstream ss;
        ss << getMsgTypeStr(messageType) << " from " << from.toString() << " to " << to.toString() << std::endl;
        return ss.str();
    }

    std::string Message::getMsgTypeStr(MsgTypes msgType)
    {
        switch (msgType)
        {
            case JOINREQ:
                return "JOINREQ";

            case JOINREP:
                return "JOINREP";

            case PING:
                return "PING";

            case ACK:
                return "ACK";

            case PING_REQ:
                return "PING_REQ";
        }

        throw std::logic_error("not impelemnted");
    }
}


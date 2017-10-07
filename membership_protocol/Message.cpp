#include <iostream>
#include <memory>
#include <sstream>

#include "Message.h"
#include "JoinReqMessage.h"
#include "JoinRepMessage.h"
#include "../proto/Message.pb.h"
#include "PingMessage.h"
#include "AckMessage.h"
#include "Events.h"
#include "Node.h"
#include "../utils/Utils.h"

namespace membership_protocol
{
    Message::Message(MsgTypes msgType, const network::Address& srcAddress, const network::Address& destAddress) :
        from(srcAddress),
        to(destAddress),
        messageType(msgType),
        id(utils::Utils::getRandomString(16))
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

        auto srcAddress = network::Address(message.sourceaddress());
        auto destAddress = network::Address(message.destinationaddress());

        switch (message.messagetype())
        {
            case gen::JOINREQ:
            {
                return std::make_unique<JoinReqMessage>(srcAddress, destAddress);
            }

            case gen::JOINREP:
            {
                return std::make_unique<JoinRepMessage>(srcAddress, destAddress);
            }

            case gen::PING:
            {
                return std::make_unique<PingMessage>(srcAddress, destAddress);
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

    const network::Address& Message::getSourceAddress() const
    {
        return from;
    }

    const network::Address& Message::getDestinationAddress() const
    {
        return to;
    }

    const std::string& Message::getId() const
    {
        return id;
    }

    network::Message Message::serialize() const
    {        
        gen::Message message = serializeToProtobuf();

        unsigned int size = message.ByteSize();
        char* data = new char[size];
        message.SerializeToArray(data, size);

        return network::Message(data, size);
    }

    gen::Message Message::serializeToProtobuf() const
    {
        auto srcAddress = from.serialize();
        auto destAddress = to.serialize();

        gen::Message message;
        message.set_messagetype(getProtobufMessageType());
        message.set_allocated_sourceaddress(srcAddress.release());
        message.set_allocated_destinationaddress(destAddress.release());

        return message;
    }

    std::string Message::toString() const
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


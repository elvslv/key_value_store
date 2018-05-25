#include <iostream>
#include <memory>
#include <sstream>

#include "AckMessage.h"
#include "JoinRepMessage.h"
#include "JoinReqMessage.h"
#include "Message.h"
#include "MessageWithGossipsBase.h"
#include "PingMessage.h"
#include "PingReqMessage.h"
#include "proto/MembershipProtocolMessage.pb.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"

namespace membership_protocol
{
Message::Message(MsgTypes msgType, const network::Address& srcAddress, const network::Address& destAddress)
    : Message(msgType, srcAddress, destAddress, utils::Utils::getRandomString(16))
{
}

Message::Message(MsgTypes msgType, const network::Address& srcAddress, const network::Address& destAddress, const std::string& id)
    : utils::Message(srcAddress, destAddress, id)
    , messageType(msgType)
{
}

Message::MsgTypes Message::getMessageType() const
{
    return messageType;
}

std::string Message::getMessageTypeDescription() const
{
    switch (messageType)
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

    default:
        throw utils::NotImplementedException();
    }
}

std::unique_ptr<Message> Message::parseMessage(const network::Message& networkMessage)
{
    gen::membership_protocol::Message message;
    message.ParseFromArray(networkMessage.content.get(), networkMessage.size);

    auto srcAddress = network::Address(message.sourceaddress());
    auto destAddress = network::Address(message.destinationaddress());

    switch (message.messagetype())
    {
    case gen::membership_protocol::JOINREQ:
    {
        return std::make_unique<JoinReqMessage>(srcAddress, destAddress, message.id());
    }

    case gen::membership_protocol::JOINREP:
    {
        auto gossips = MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<JoinRepMessage>(srcAddress, destAddress, gossips, message.id());
    }

    case gen::membership_protocol::PING:
    {
        auto gossips = MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<PingMessage>(srcAddress, destAddress, gossips, message.id());
    }

    case gen::membership_protocol::ACK:
    {
        if (!message.has_ackfields())
        {
            throw std::logic_error("AckFields are expected");
        }

        auto ackFields = message.ackfields();
        auto gossips = MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<AckMessage>(srcAddress, destAddress, gossips, message.id(), ackFields.originalid());
    }

    case gen::membership_protocol::PING_REQ:
    {
        if (!message.has_pingreqfields())
        {
            throw std::logic_error("PingReq fields are expected");
        }

        auto pingReqFields = message.pingreqfields();
        auto gossips = MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<PingReqMessage>(srcAddress, destAddress, message.id(), gossips, pingReqFields.targetaddress());
    }
    }

    throw std::logic_error("not impelemnted");
}

network::Message Message::serialize() const
{
    gen::membership_protocol::Message message = serializeToProtobuf();

    unsigned int size = message.ByteSize();
    char* data = new char[size];
    message.SerializeToArray(data, size);

    return network::Message(data, size);
}

gen::membership_protocol::Message Message::serializeToProtobuf() const
{
    auto srcAddress = sourceAddress.serialize();
    auto destAddress = destinationAddress.serialize();

    gen::membership_protocol::Message message;
    message.set_messagetype(getProtobufMessageType());
    message.set_allocated_sourceaddress(srcAddress.release());
    message.set_allocated_destinationaddress(destAddress.release());
    message.set_id(id);

    return message;
}

std::string Message::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr(messageType) << " from " << sourceAddress.toString() << " to " << destinationAddress.toString() << "id " << id << std::endl;
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

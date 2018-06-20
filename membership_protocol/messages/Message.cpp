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

std::string Message::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr() << " from " << sourceAddress.toString() << " to " << destinationAddress.toString() << "id " << id << std::endl;
    return ss.str();
}

gen::membership_protocol::Message* Message::getMembershipProtocolMessage(gen::Message& message)
{
    if (message.has_membershipprotocolmessage())
    {
        return message.mutable_membershipprotocolmessage();
    }

    throw std::logic_error("membershipt protocol message is not set");
}

const gen::membership_protocol::Message& Message::getMembershipProtocolMessage(const gen::Message& message)
{
    if (message.has_membershipprotocolmessage())
    {
        return message.membershipprotocolmessage();
    }

    throw std::logic_error("membershipt protocol message is not set");
}

gen::Message Message::serializeToProtobuf() const
{
    auto message = utils::Message::serializeToProtobuf();

    auto membershipProtocolMessage = std::make_unique<gen::membership_protocol::Message>();
    membershipProtocolMessage->set_messagetype(getProtobufMessageType());
    message.set_allocated_membershipprotocolmessage(membershipProtocolMessage.release());

    return message;
}
}

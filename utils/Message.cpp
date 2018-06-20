#include "Message.h"
#include "Exceptions.h"
#include "Utils.h"
#include "key_value_store/CreateRequestMessage.h"
#include "key_value_store/CreateResponseMessage.h"
#include "key_value_store/DeleteRequestMessage.h"
#include "key_value_store/DeleteResponseMessage.h"
#include "key_value_store/ReadRequestMessage.h"
#include "key_value_store/ReadResponseMessage.h"
#include "key_value_store/UpdateRequestMessage.h"
#include "key_value_store/UpdateResponseMessage.h"
#include "membership_protocol/messages/AckMessage.h"
#include "membership_protocol/messages/JoinRepMessage.h"
#include "membership_protocol/messages/JoinReqMessage.h"
#include "membership_protocol/messages/MessageWithGossipsBase.h"
#include "membership_protocol/messages/PingMessage.h"
#include "membership_protocol/messages/PingReqMessage.h"

#include <memory>

namespace utils
{
Message::Message(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& id)
    : sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , id(id)
{
}

Message::Message(const network::Address& sourceAddress, const network::Address& destinationAddress)
    : sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , id(utils::Utils::getRandomString(16))
{
}

network::Address Message::getSourceAddress() const
{
    return sourceAddress;
}

network::Address Message::getDestinationAddress() const
{
    return destinationAddress;
}

std::string Message::getId() const
{
    return id;
}

std::string Message::getMsgTypeStr() const
{
    return typeid(*this).name();
}

std::unique_ptr<Message> Message::parseMessage(const network::Message& networkMessage)
{
    gen::Message message;
    message.ParseFromArray(networkMessage.content.get(), networkMessage.size);

    network::Address srcAddress(message.sourceaddress());
    network::Address destAddress(message.destinationaddress());
    auto id = message.id();

    if (message.has_keyvaluestoremessage())
    {
        auto kvStoreMessage = message.keyvaluestoremessage();
        if (kvStoreMessage.has_requestmessage())
        {
            auto requestMessage = kvStoreMessage.requestmessage();
            auto key = requestMessage.key();

            switch (requestMessage.messagetype())
            {
            case gen::key_value_store::CREATE_REQUEST:
            {
                if (!requestMessage.has_createfields())
                {
                    throw std::logic_error("CreateFields are expected");
                }

                auto createFields = requestMessage.createfields();
                return std::make_unique<key_value_store::CreateRequestMessage>(srcAddress, destAddress, key, createFields.value(), id);
            }

            case gen::key_value_store::UPDATE_REQUEST:
            {
                if (!requestMessage.has_updatefields())
                {
                    throw std::logic_error("UpdateFields are expected");
                }

                auto updateFields = requestMessage.updatefields();
                return std::make_unique<key_value_store::UpdateRequestMessage>(srcAddress, destAddress, key, updateFields.value(), id);
            }

            case gen::key_value_store::READ_REQUEST:
            {
                return std::make_unique<key_value_store::ReadRequestMessage>(srcAddress, destAddress, key, id);
            }

            case gen::key_value_store::DELETE_REQUEST:
            {
                return std::make_unique<key_value_store::DeleteRequestMessage>(srcAddress, destAddress, key, id);
            }
            }
        }
        else
        {
            assert(kvStoreMessage.has_responsemessage());
            auto responseMessage = kvStoreMessage.responsemessage();
            switch (responseMessage.messagetype())
            {
            case gen::key_value_store::CREATE_RESPONSE:
            {
                return std::make_unique<key_value_store::CreateResponseMessage>(srcAddress, destAddress, responseMessage.originalmessageid(), responseMessage.responsecode(), id);
            }

            case gen::key_value_store::UPDATE_RESPONSE:
            {
                return std::make_unique<key_value_store::UpdateResponseMessage>(srcAddress, destAddress, responseMessage.originalmessageid(), responseMessage.responsecode(), id);
            }
            case gen::key_value_store::READ_RESPONSE:
            {
                if (!responseMessage.has_readfields())
                {
                    throw std::logic_error("ReadFields are expected");
                }

                auto readFields = responseMessage.readfields();
                return std::make_unique<key_value_store::ReadResponseMessage>(srcAddress, destAddress, responseMessage.originalmessageid(), responseMessage.responsecode(), readFields.value(), id);
            }
            case gen::key_value_store::DELETE_RESPONSE:
            {
                return std::make_unique<key_value_store::DeleteResponseMessage>(srcAddress, destAddress, responseMessage.originalmessageid(), responseMessage.responsecode(), id);
            }
            }
        }

        throw utils::NotImplementedException();
    }

    assert(message.has_membershipprotocolmessage());
    auto mpMessage = message.membershipprotocolmessage();
    switch (mpMessage.messagetype())
    {
    case gen::membership_protocol::JOINREQ:
    {
        return std::make_unique<membership_protocol::JoinReqMessage>(srcAddress, destAddress, message.id());
    }

    case gen::membership_protocol::JOINREP:
    {
        auto gossips = membership_protocol::MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<membership_protocol::JoinRepMessage>(srcAddress, destAddress, gossips, message.id());
    }

    case gen::membership_protocol::PING:
    {
        auto gossips = membership_protocol::MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<membership_protocol::PingMessage>(srcAddress, destAddress, gossips, message.id());
    }

    case gen::membership_protocol::ACK:
    {
        if (!mpMessage.has_ackfields())
        {
            throw std::logic_error("AckFields are expected");
        }

        auto ackFields = mpMessage.ackfields();
        auto gossips = membership_protocol::MessageWithGossipsBase::parseGossips(message);
        return std::make_unique<membership_protocol::AckMessage>(srcAddress, destAddress, gossips, message.id(), ackFields.originalid());
    }

    case gen::membership_protocol::PING_REQ:
    {
        if (!mpMessage.has_pingreqfields())
        {
            throw std::logic_error("PingReq fields are expected");
        }

        auto pingReqFields = mpMessage.pingreqfields();
        auto gossips = membership_protocol::MessageWithGossipsBase::parseGossips(message);
        network::Address targetAddress(pingReqFields.targetaddress());
        return std::make_unique<membership_protocol::PingReqMessage>(srcAddress, destAddress, message.id(), gossips, targetAddress);
    }
    }

    throw utils::NotImplementedException();
}

gen::Message Message::serializeToProtobuf() const
{
    auto srcAddress = sourceAddress.serialize();
    auto destAddress = destinationAddress.serialize();

    gen::Message message;
    message.set_allocated_sourceaddress(srcAddress.release());
    message.set_allocated_destinationaddress(destAddress.release());
    message.set_id(id);

    return message;
}

network::Message Message::serialize() const
{
    gen::Message message = serializeToProtobuf();

    unsigned int size = message.ByteSize();
    char* data = new char[size];
    message.SerializeToArray(data, size);

    return network::Message(data, size);
}
}
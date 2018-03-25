#include "RequestMessage.h"
#include "CreateRequestMessage.h"
#include "DeleteRequestMessage.h"
#include "ReadRequestMessage.h"
#include "UpdateRequestMessage.h"
#include "utils/Exceptions.h"

#include <memory>
#include <sstream>

namespace key_value_store
{
RequestMessage::RequestMessage(RequestMessageType messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& id)
    : messageType(messageType)
    , sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , key(key)
    , id(id)
{
}

RequestMessage::RequestMessage(RequestMessageType messageType, const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key)
    : messageType(messageType)
    , sourceAddress(sourceAddress)
    , destinationAddress(destinationAddress)
    , key(key)
{
}

RequestMessageType RequestMessage::getMessageType() const
{
    return messageType;
}

network::Address RequestMessage::getDestinationAddress() const
{
    return destinationAddress;
}

network::Address RequestMessage::getSourceAddress() const
{
    return sourceAddress;
}

std::string RequestMessage::getKey() const
{
    return key;
}

std::string RequestMessage::getId() const
{
    return id;
}

std::unique_ptr<RequestMessage> RequestMessage::parseMessage(const network::Message& networkMessage)
{
    gen::RequestMessage message;
    message.ParseFromArray(networkMessage.content.get(), networkMessage.size);

    auto srcAddress = network::Address(message.sourceaddress());
    auto destAddress = network::Address(message.destinationaddress());
    auto key = message.key();
    auto id = message.id();

    switch (message.messagetype())
    {
    case gen::CREATE_REQUEST:
    {
        if (!message.has_createfields())
        {
            throw std::logic_error("CreateFields are expected");
        }

        auto createFields = message.createfields();
        return std::make_unique<CreateRequestMessage>(srcAddress, destAddress, key, createFields.value(), id);
    }

    case gen::UPDATE_REQUEST:
    {
        if (!message.has_updatefields())
        {
            throw std::logic_error("UpdateFields are expected");
        }

        auto updateFields = message.updatefields();
        return std::make_unique<UpdateRequestMessage>(srcAddress, destAddress, key, updateFields.value(), id);
    }

    case gen::READ_REQUEST:
    {
        return std::make_unique<ReadRequestMessage>(srcAddress, destAddress, key, id);
    }

    case gen::DELETE_REQUEST:
    {
        return std::make_unique<DeleteRequestMessage>(srcAddress, destAddress, key, id);
    }
    }

    throw utils::NotImplementedException();
}

std::string RequestMessage::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr(messageType) << " request from " << sourceAddress.toString() << " to " << sourceAddress.toString() << "id " << id << std::endl;
    return ss.str();
}

network::Message RequestMessage::serialize() const
{
    gen::RequestMessage message = serializeToProtobuf();

    unsigned int size = message.ByteSize();
    char* data = new char[size];
    message.SerializeToArray(data, size);

    return network::Message(data, size);
}
gen::RequestMessage RequestMessage::serializeToProtobuf() const
{
    auto srcAddress = sourceAddress.serialize();
    auto destAddress = destinationAddress.serialize();

    gen::RequestMessage message;
    message.set_messagetype(getProtobufMessageType());
    message.set_allocated_sourceaddress(srcAddress.release());
    message.set_allocated_destinationaddress(destAddress.release());
    message.set_id(id);
    message.set_key(key);

    return message;
}

gen::RequestMessageType RequestMessage::getProtobufMessageType() const
{
    switch (messageType)
    {
    case CREATE:
        return gen::CREATE_REQUEST;

    case UPDATE:
        return gen::UPDATE_REQUEST;

    case READ:
        return gen::READ_REQUEST;

    case DELETE:
        return gen::DELETE_REQUEST;
    }

    throw utils::NotImplementedException();
}

std::string RequestMessage::getMsgTypeStr(RequestMessageType msgType)
{
    switch (msgType)
    {
    case CREATE:
        return "CREATE";

    case UPDATE:
        return "UPDATE";

    case READ:
        return "READ";

    case DELETE:
        return "DELETE";
    }

    throw utils::NotImplementedException();
}
}
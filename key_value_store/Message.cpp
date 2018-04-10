#include "Message.h"
#include "CreateRequestMessage.h"
#include "DeleteRequestMessage.h"
#include "ReadRequestMessage.h"
#include "RequestMessage.h"
#include "UpdateRequestMessage.h"
#include "utils/Exceptions.h"

#include <sstream>

namespace key_value_store
{
std::unique_ptr<Message> Message::parseMessage(const network::Message& networkMessage)
{
    gen::Message message;
    message.ParseFromArray(networkMessage.content.get(), networkMessage.size);

    auto srcAddress = network::Address(message.sourceaddress());
    auto destAddress = network::Address(message.destinationaddress());
    auto id = message.id();

    if (message.has_requestmessage())
    {
        auto requestMessage = message.requestmessage();
        auto key = requestMessage.key();

        switch (requestMessage.messagetype())
        {
        case gen::CREATE_REQUEST:
        {
            if (!requestMessage.has_createfields())
            {
                throw std::logic_error("CreateFields are expected");
            }

            auto createFields = requestMessage.createfields();
            return std::make_unique<CreateRequestMessage>(srcAddress, destAddress, key, createFields.value(), id);
        }

        case gen::UPDATE_REQUEST:
        {
            if (!requestMessage.has_updatefields())
            {
                throw std::logic_error("UpdateFields are expected");
            }

            auto updateFields = requestMessage.updatefields();
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
    }

    throw utils::NotImplementedException();
}

gen::Message Message::serializeToProtobuf() const
{
    auto srcAddress = sourceAddress.serialize();
    auto destAddress = destinationAddress.serialize();

    auto message = Message::serializeToProtobuf();
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

std::string Message::toString() const
{
    std::stringstream ss;
    ss << getMsgTypeStr() << " request from " << sourceAddress.toString() << " to " << destinationAddress.toString() << "id " << id << std::endl;
    return ss.str();
}
}
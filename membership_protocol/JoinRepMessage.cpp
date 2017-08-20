#include <sstream>
#include "JoinRepMessage.h"
#include "../build/gen/Message.pb.h"

namespace membership_protocol
{
    JoinRepMessage::JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Node>& members):
        Message(JOINREP, from, to),
        memberList(members)
    {
    }

    std::string JoinRepMessage::toString()
    {
        std::stringstream ss;
        ss << Message::toString() << std::endl;
        ss << "Members list: " << std::endl;
        for (std::vector<Node>::iterator it = memberList.begin(); it != memberList.end(); ++it)
        {
            ss << it->getAddress().toString() << std::endl;
        }

        return ss.str();
    }

    gen::MessageType JoinRepMessage::getProtobufMessageType()
    {
        return gen::JOINREP;
    }

    gen::Message JoinRepMessage::serializeToProtobuf()
    {
        gen::Message message = Message::serializeToProtobuf();
        auto list = message.mutable_members();
        list->Reserve(memberList.size());

        for (int i = 0; i < memberList.size(); ++i)
        {
            auto address = memberList[i].getAddress().serialize();
        
            auto entity = new gen::Node;
            entity->set_allocated_address(address.release());
            list->AddAllocated(entity);
        }

        return message;
    }

    const std::vector<Node>& JoinRepMessage::getMemberList()
    {
        return memberList;
    }
}
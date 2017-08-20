#pragma once
#include "Message.h"
#include "Node.h"

namespace membership_protocol
{
    class JoinRepMessage: public Message
    {
    public:
        JoinRepMessage(const network::Address& from, const network::Address& to, const std::vector<Node>& members);
        std::string toString();
        virtual gen::MessageType getProtobufMessageType();
        virtual gen::Message serializeToProtobuf();
        const std::vector<Node>& getMemberList();
    private:
        std::vector<Node> memberList;
    };
}

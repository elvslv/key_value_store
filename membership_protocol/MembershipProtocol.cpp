#include "MembershipProtocol.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr): 
        node(addr),
        members(),
        network(addr)
    {
    }

    const std::vector<Node>& MembershipProtocol::getMembers() const
    {
        return members;
    }

    void MembershipProtocol::sendMessage(std::unique_ptr<Message> message, std::unique_ptr<Address> destAddress)
    {
        int msgSize;
        char* msg = message->serialize(msgSize);
        log("Sending message", message->toString());

        emulNet->ENsend(std::make_unique<Address>(memberNode->addr), std::move(destAddress), msg, msgSize);

        free(msg);
    }
}
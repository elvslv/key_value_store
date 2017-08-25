#include "MembershipProtocol.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger): 
        node(addr),
        members(),
        network(addr),
        logger(logger)
    {
    }

    const std::vector<Node>& MembershipProtocol::getMembers() const
    {
        return members;
    }

    void MembershipProtocol::sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress)
    {
        auto msg = message->serialize();
        log("Sending message", msg.content.get());

        network.send(destAddress, msg);
    }
}
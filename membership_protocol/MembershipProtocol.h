#pragma once

#include <memory>
#include <vector>
#include "Node.h"
#include "Message.h"
#include "../network/Network.h"
#include "../utils/Log.h"

namespace membership_protocol
{
    class MembershipProtocol
    {
    public:
        MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger);
        const std::vector<Node>& getMembers() const;
    private:
        Node node;
        std::vector<Node> members; 
        network::Network network;
        std::shared_ptr<utils::Log> logger;

        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            logger->log(node.getAddress(), next...);
        }

        void sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress);
    };
}
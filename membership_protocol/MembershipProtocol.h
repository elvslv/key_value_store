#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <list>
#include "Node.h"
#include "Message.h"
#include "Events.h"
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
        std::deque<Event> events;
        std::list<std::pair<long, Node> > waitingList;
        std::map<std::string, std::list<std::pair<long, Node> >::iterator > waitingListIter;
        
        network::Network network;
        std::shared_ptr<utils::Log> logger;

        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            logger->log(node.getAddress().toString(), next...);
        }

        const long PING_TIMEOUT = 1000;

        void sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress);
        void processMessage(const std::unique_ptr<Message> & message);
        void addMemberListEntry(const network::Address& address);
        void executeMembershipProtocol();
    };
}
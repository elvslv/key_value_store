#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <list>
#include <unordered_map>
#include <unordered_set>
// #include "Node.h"
#include "Message.h"
#include "Events.h"
#include "MembershipUpdate.h"
#include "../network/Network.h"
#include "../utils/Log.h"
#include "IMembershipProtocol.h"

namespace membership_protocol
{
    class MembershipProtocol: public IMembershipProtocol
    {
    public:
        MembershipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger);
        virtual void start();
        virtual void stop();
        virtual std::vector<Member> getMembers();
    private:
        network::Address node;
        network::Network network;
        std::shared_ptr<utils::Log> logger;
        std::mutex mutex;

        std::list<network::Address> members; 
        // std::unordered_set<network::Address> membersSet;

        // membership updates
        std::unordered_map<network::Address, int> membersMap; 
        std::map<int, std::unordered_map<network::Address, Event> > events;
        
        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            logger->log(node.toString(), next...);
        }

        const long PING_TIMEOUT = 1000;

        void sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress);
        void processMessage(const std::unique_ptr<Message> & message);

        void processEvent(const Event& event);

        void addMember(const network::Address& address);
        // void addMembers(const std::vector<network::Address>& nodes);
        void executeMembershipProtocol();

        void logNodeAdded(const network::Address& nodeAddr);
        void logNodeRemoved(const network::Address& nodeAddr);        
    };
}
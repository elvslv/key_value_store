#pragma once

#include <vector>
#include "Node.h"
#include "../network/Network.h"

namespace membership_protocol
{
    class MembershipProtocol
    {
    public:
        MembershipProtocol(const network::Address& addr);
        const std::vector<Node>& getMembers() const;
    private:
        Node node;
        std::vector<Node> members; 
        network::Network network;

        template <typename T >
        void log_(std::stringstream& ss, T only)
        {
            ss << only << std::endl;
        }


        template <typename T, typename ... args >
        void log_(std::stringstream& ss, T current, args... next )
        {
            ss << current << ' ';
            log_(ss, next... );
        }

        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            std::stringstream ss;
            log_(ss, current, next...);

            const char* str = ss.str().c_str();
            logger->LOG(&memberNode->addr, str);
        }
    };
}
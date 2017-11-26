#pragma once

#include "Member.h"
#include "MembershipUpdate.h"

namespace membership_protocol
{
    class IMembershipProtocol 
    {
    public:
        class IObserver
        {
        public:
            virtual ~IObserver(){}
            virtual void onMembershipUpdate(const MembershipUpdate& membershipUpdate) = 0;
        };

        virtual ~IMembershipProtocol(){}
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual std::vector<Member> getMembers() = 0;
        virtual void addObserver(IMembershipProtocol::IObserver* observer) = 0; 
        virtual size_t getMembersNum() = 0;
    };
}

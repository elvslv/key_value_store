#include "FailureDetector.h"
#include "../utils/Exceptions.h"

namespace membership_protocol
{
    FailureDetector::FailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, IMembershipProtocol* membershipProtocol):
        address(addr),
        logger(logger),
        messageDispatcher(messageDispatcher),
        membershipProtocol(membershipProtocol),
        observers(),
        membersMutex(),
        members(),
        membersMap(),
        currentMember(members.end())
    {
    }
    
    void FailureDetector::start()
    {

    }

    void FailureDetector::stop()
    {

    }

    void FailureDetector::addObserver(IFailureDetector::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void FailureDetector::onMembershipUpdate(const MembershipUpdate& membershipUpdate)
    {
        auto address = membershipUpdate.member.address;
        switch (membershipUpdate.updateType)
        {
            case JOINED:
            {
                std::lock_guard<std::mutex> lock(membersMutex);
                if (membersMap.find(address) != membersMap.end())
                {
                    throw std::logic_error("Member " + address.toString() + " has been already added");
                }

                // test for corner cases!!!
                int shift = rand() % members.size();
                auto it = members.begin();
                std::advance(it, shift);

                it = members.insert(it, address);
                membersMap.insert(address, it);

                logger->log("Successfully inserted node ", address.toString(), " into position ", shift);

                break;
            }
            
            case FAILED:
            {
                std::lock_guard<std::mutex> lock(membersMutex);

                auto it = membersMap.find(address);
                if (it == membersMap.end())
                {
                    throw std::logic_error("Member " + address.toString() + " wasn't found");
                }

                if (it->second == currentMember)
                {
                    throw utils::NotImplementedException();
                }

                members.erase(it->second);
                membersMap.erase(it);

                break;                
            }
        }
    }
}
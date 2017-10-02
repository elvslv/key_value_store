#include <list>
#include <unordered_map>

#include "IFailureDetector.h"
#include "../utils/Log.h"
#include "../utils/MessageDispatcher.h"
#include "IMembershipProtocol.h"

namespace membership_protocol
{
    class FailureDetector: public IFailureDetector, IMembershipProtocol::IObserver
    {
    public:
        FailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, IMembershipProtocol* membershipProtocol);

        virtual void start();
        virtual void stop();
        virtual void addObserver(IFailureDetector::IObserver* observer);

        virtual void onMembershipUpdate(const MembershipUpdate& membershipUpdate);
    private:
        network::Address address;
        std::shared_ptr<utils::Log> logger;
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
        membership_protocol::IMembershipProtocol* membershipProtocol;
        std::vector<IFailureDetector::IObserver*> observers;
        
        std::mutex membersMutex;
        std::list<network::Address> members;
        std::unordered_map<network::Address, std::list<network::Address>::iterator> membersMap;
        std::list<network::Address>::iterator currentMember;
    };
}
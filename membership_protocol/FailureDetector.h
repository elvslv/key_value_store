#include <list>
#include <unordered_map>

#include "IFailureDetector.h"
#include "../utils/AsyncQueue.h"
#include "../utils/Log.h"
#include "../utils/MessageDispatcher.h"
#include "../utils/RoundRobinList.h"
#include "IMembershipProtocol.h"

namespace membership_protocol
{
    class FailureDetector: public IFailureDetector, public IMembershipProtocol::IObserver
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
        std::unordered_map<MsgTypes, std::string> tokens;        
        membership_protocol::IMembershipProtocol* membershipProtocol;
        std::vector<IFailureDetector::IObserver*> observers;
        utils::AsyncQueue asyncQueue;
        utils::AsyncQueue::Callback asyncQueueCallback;
        
        utils::RoundRobinList<network::Address> members;
        std::unique_ptr<std::thread> messageProcessingThread;
        volatile bool isRunning;

        std::mutex msgIdsMutex;
        std::unordered_map<std::string, bool> msgIds;

        void run();
        void processMessage(const std::unique_ptr<Message>& message);
        void sendPing(const network::Address destAddress);
    };
}
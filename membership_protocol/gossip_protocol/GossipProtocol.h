#include <list>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "IGossipProtocol.h"
#include "utils/AsyncQueue.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"
#include "utils/RoundRobinList.h"
#include "IMembershipProtocol.h"

namespace membership_protocol
{
    struct Gossip
    {
        int period;
        std::string id;
        MembershipUpdate membershipUpdate;
        std::unordered_set<network::Address> infectedNodes;
    };

    class GossipProtocol: public IGossipProtocol, public IMembershipProtocol::IObserver
    {
    public:
        GossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, IMembershipProtocol* membershipProtocol);
        
        virtual void start();
        virtual void stop();
        virtual void addObserver(IGossipProtocol::IObserver* observer);
        virtual void spreadMembershipUpdate(const MembershipUpdate& membershipUpdate);
        virtual void onMembershipUpdate(const MembershipUpdate& membershipUpdate);
    private:
        network::Address address;
        std::shared_ptr<utils::Log> logger;
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
        std::unordered_map<MsgTypes, std::string> tokens;
        membership_protocol::IMembershipProtocol* membershipProtocol;
        std::vector<IGossipProtocol::IObserver*> observers;
        utils::AsyncQueue asyncQueue;
        utils::AsyncQueue::Callback asyncQueueCallback;

        utils::RoundRobinList<network::Address> members;
        std::unique_ptr<std::thread> messageProcessingThread;
        volatile bool isRunning;

        volatile int period;

        std::unordered_map<std::string, Gossip> gossips;
        std::unordered_map<int, std::unordered_set<std::string> > periods;
        std::mutex gossipsMutex;

        void run();
        void processMessage(const std::unique_ptr<Message>& message);
    };
}
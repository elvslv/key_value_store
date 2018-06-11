#include <list>
#include <string>
#include <unordered_map>

#include "IFailureDetector.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "membership_protocol/gossip_protocol/IGossipProtocol.h"
#include "membership_protocol/messages/PingReqMessage.h"
#include "utils/AsyncQueue.h"
#include "utils/IThreadPolicy.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"
#include "utils/RoundRobinList.h"
#include "utils/Runnable.h"

namespace failure_detector
{
class FailureDetector : public IFailureDetector, public membership_protocol::IMembershipProtocol::IObserver
{
public:
    FailureDetector(const network::Address& addr, std::shared_ptr<utils::Log> logger, std::shared_ptr<utils::MessageDispatcher<membership_protocol::Message>> messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy);
    virtual ~FailureDetector();

    virtual void start();
    virtual void stop();
    virtual void addObserver(IFailureDetector::IObserver* observer);
    virtual void requestStop();

    virtual void onMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate);

private:
    network::Address address;
    std::shared_ptr<utils::Log> logger;
    std::shared_ptr<utils::MessageDispatcher<membership_protocol::Message>> messageDispatcher;
    std::unordered_map<membership_protocol::Message::MsgTypes, std::string> tokens;
    membership_protocol::IMembershipProtocol* membershipProtocol;
    gossip_protocol::IGossipProtocol* gossipProtocol;
    std::shared_ptr<utils::IThreadPolicy> threadPolicy;
    std::vector<IFailureDetector::IObserver*> observers;
    utils::AsyncQueue<membership_protocol::Message> asyncQueue;
    utils::AsyncQueue<membership_protocol::Message>::Callback asyncQueueCallback;

    utils::RoundRobinList<network::Address> members;

    std::mutex msgIdsMutex;
    std::condition_variable ackReceivedCondVar;
    std::unordered_map<std::string, bool> msgIds;

    utils::RunnableCallback runnable;
    std::list<std::thread> pingReqThreads;
    bool stopped;
    bool stopRequested;

    void run();
    void onPingReq(std::unique_ptr<membership_protocol::Message> message);
    void processMessage(std::unique_ptr<membership_protocol::Message> message);
    bool sendPing(const network::Address& destAddress, int k);
    bool sendPingReq(const network::Address& destAddress, const network::Address& target);
    bool sendMessage(std::unique_ptr<membership_protocol::Message> message, const network::Address& destAddress, int timeoutSeconds);
};
}
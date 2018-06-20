#pragma once

#include "Config.h"
#include "IMembershipProtocol.h"
#include "Member.h"
#include "failure_detector/IFailureDetector.h"
#include "failure_detector/IFailureDetectorFactory.h"
#include "gossip_protocol/IGossipProtocol.h"
#include "gossip_protocol/IGossipProtocolFactory.h"
#include "messages/Message.h"
#include "utils/AsyncQueue.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"
#include <queue>
#include <unordered_map>
#include <list>

namespace membership_protocol
{
class MembershipProtocol : public IMembershipProtocol, failure_detector::IFailureDetector::IObserver, gossip_protocol::IGossipProtocol::IObserver
{
public:
    MembershipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger, const Config& config, std::shared_ptr<utils::MessageDispatcher> messageDispatcher, const std::unique_ptr<failure_detector::IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<gossip_protocol::IGossipProtocolFactory>& gossipProtocolFactory, std::shared_ptr<utils::IThreadPolicy> threadPolicy);
    virtual ~MembershipProtocol();

    virtual void start();
    virtual void stop();
    virtual void requestStop();

    virtual std::vector<Member> getMembers();
    virtual size_t getMembersNum();
    virtual void addObserver(IMembershipProtocol::IObserver* observer);

    virtual void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent);
    virtual void onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate);

private:
    network::Address node;
    Config config;
    std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
    std::list<std::string> tokens;
    std::shared_ptr<utils::Log> logger;
    utils::AsyncQueue<membership_protocol::Message> asyncQueue;
    utils::AsyncQueue<utils::Message>::Callback asyncQueueCallback;

    std::unique_ptr<gossip_protocol::IGossipProtocol> gossipProtocol;
    std::unique_ptr<failure_detector::IFailureDetector> failureDetector;
    std::vector<IMembershipProtocol::IObserver*> observers;

    std::mutex membersMutex;
    std::unordered_map<std::string, Member> members;

    std::atomic<bool> joined;
    bool stopped;

    void onMembershipUpdate(const MembershipUpdate& membershipUpdate, MembershipUpdateSource membershipUpdateSource);
    void onMembershipUpdate(MembershipUpdateType membershipUpdateType, MembershipUpdateSource membershipUpdateSource, const network::Address& sourceAddress);

    void sendMessage(std::unique_ptr<Message> message, const network::Address& destAddress);

    void onJoin();
    void addMember(const network::Address& address);

    void processMessage(std::unique_ptr<Message> message);

    template <typename T, typename... args>
    void log(T current, args... next)
    {
        logger->log(node.toString(), current, next...);
    }
};
}
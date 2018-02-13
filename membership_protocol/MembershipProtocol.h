#pragma once

#include <queue>
#include <unordered_map>
#include "IMembershipProtocol.h"
#include "failure_detector/IFailureDetector.h"
#include "gossip_protocol/IGossipProtocol.h"
#include "failure_detector/IFailureDetectorFactory.h"
#include "gossip_protocol/IGossipProtocolFactory.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"
#include "utils/AsyncQueue.h"
#include "Member.h"
#include "messages/Message.h"

namespace membership_protocol
{
    class MembershipProtocol: public IMembershipProtocol, failure_detector::IFailureDetector::IObserver, gossip_protocol::IGossipProtocol::IObserver
    {
    public:
        MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, const std::unique_ptr<failure_detector::IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<gossip_protocol::IGossipProtocolFactory>& gossipProtocolFactory);
        virtual ~MembershipProtocol();

        virtual void start();
        virtual void stop();
        virtual std::vector<Member> getMembers();
        virtual size_t getMembersNum();
        virtual void addObserver(IMembershipProtocol::IObserver* observer);

        virtual void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent);
        virtual void onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate);
    private:
        network::Address node;
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
        std::unordered_map<MsgTypes, std::string> tokens;
        std::shared_ptr<utils::Log> logger;
        utils::AsyncQueue asyncQueue;
        utils::AsyncQueue::Callback asyncQueueCallback;

        std::unique_ptr<gossip_protocol::IGossipProtocol> gossipProtocol;
        std::unique_ptr<failure_detector::IFailureDetector> failureDetector;
        std::vector<IMembershipProtocol::IObserver*> observers;

        std::mutex membersMutex;
        std::unordered_map<std::string, Member> members;

        std::atomic<bool> joined;
        bool stopped;

        void onMembershipUpdate(const MembershipUpdate& membershipUpdate, MembershipUpdateSource membershipUpdateSource);
        void onMembershipUpdate(MembershipUpdateType membershipUpdateType, MembershipUpdateSource membershipUpdateSource, const network::Address& sourceAddress);

        void sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress);        

        network::Address getJoinAddress();
        void onJoin();
        void addMember(const network::Address& address);

        void processMessage(const std::unique_ptr<Message>& message);

        template <typename T, typename ... args >
        void log(T current, args... next )
        {
            logger->log(node.toString(), current, next...);
        }
    };
}
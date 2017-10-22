#pragma once

#include <queue>
#include <unordered_map>
#include "IMembershipProtocol.h"
#include "IFailureDetector.h"
#include "IGossipProtocol.h"
#include "IFailureDetectorFactory.h"
#include "IGossipProtocolFactory.h"
#include "../utils/Log.h"
#include "../utils/MessageDispatcher.h"
#include "../utils/AsyncQueue.h"
#include "Member.h"
#include "messages/Message.h"

namespace membership_protocol
{
    class MembershipProtocol: public IMembershipProtocol, IFailureDetector::IObserver, IGossipProtocol::IObserver
    {
    public:
        MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::unique_ptr<IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<IGossipProtocolFactory>& gossipProtocolFactory);
        virtual ~MembershipProtocol() {}

        virtual void start();
        virtual void stop();
        virtual std::vector<Member> getMembers();
        virtual void addObserver(IMembershipProtocol::IObserver* observer);

        virtual void onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent);
        virtual void onGossipEvent(const gossip_protocol::GossipEvent& gossipEvent);
    private:
        network::Address node;
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
        std::unordered_map<MsgTypes, std::string> tokens;
        std::shared_ptr<utils::Log> logger;
        utils::AsyncQueue asyncQueue;
        utils::AsyncQueue::Callback asyncQueueCallback;

        std::unique_ptr<IFailureDetector> failureDetector;
        std::unique_ptr<IGossipProtocol> gossipProtocol;
        std::vector<IMembershipProtocol::IObserver*> observers;

        std::mutex membersMutex;
        std::unordered_map<std::string, Member> members;

        volatile bool joined;

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
            logger->log(node.toString(), next...);
        }
    };
}
#include <sstream>

#include "MembershipProtocol.h"
#include "MembershipUpdate.h"
#include "messages/JoinReqMessage.h"
#include "messages/JoinRepMessage.h"
#include "messages/AckMessage.h"
#include "utils/Exceptions.h"


namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, const std::unique_ptr<failure_detector::IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<gossip_protocol::IGossipProtocolFactory>& gossipProtocolFactory): 
        node(addr),
        messageDispatcher(messageDispatcher),
        tokens(),
        logger(logger),
        asyncQueue(std::bind(&MembershipProtocol::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<Message> message){asyncQueue.push(std::move(message));}),
        gossipProtocol(gossipProtocolFactory->createGossipProtocol(addr, logger, this)),
        failureDetector(failureDetectorFactory->createFailureDetector(addr, logger, messageDispatcher, this, gossipProtocol.get())),
        observers(),
        membersMutex(),
        members(),
        joined(false),
        stopped(false)
    {
    }

    MembershipProtocol::~MembershipProtocol()
    {
        stop();

        log("[~FailureDetector]");
    }

    void MembershipProtocol::start()
    {
        log("[MembershipProtocol::start]");

        asyncQueue.start();
        messageDispatcher->start();

        gossipProtocol->addObserver(this);
        failureDetector->addObserver(this);

        // is it a good place?
        tokens[PING] = messageDispatcher->listen(PING, asyncQueueCallback);

        auto targetAddress = getJoinAddress();
        if (targetAddress == node)
        {
            tokens[JOINREQ] = messageDispatcher->listen(JOINREQ, asyncQueueCallback);
            onJoin();
            return;
        }

        tokens[JOINREP] = messageDispatcher->listen(JOINREP, asyncQueueCallback);
        messageDispatcher->sendMessage(std::make_unique<JoinReqMessage>(node, targetAddress), targetAddress);
    }

    void MembershipProtocol::stop()
    {
        if (stopped)
        {
            return;
        }

        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            messageDispatcher->stopListening(it->first, it->second);
        }

        // finish processing async queue

        failureDetector->stop();    
        gossipProtocol->stop();

        messageDispatcher->stop();
        asyncQueue.stop();

        stopped = true;
        log("[MembershipProtocol::stop]");
    }

    std::vector<Member> MembershipProtocol::getMembers()
    {
        std::vector<Member> result;
        std::lock_guard<std::mutex> lock(membersMutex);
        for (auto it = members.begin(); it != members.end(); ++it)
        {
            result.push_back(it->second);
        }

        return result;
    }

    size_t MembershipProtocol::getMembersNum()
    {
        std::lock_guard<std::mutex> lock(membersMutex);
        return members.size();
    }

    void MembershipProtocol::addObserver(IMembershipProtocol::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void MembershipProtocol::onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent)
    {
        MembershipUpdateType membershipUpdateType;
        switch (failureDetectorEvent.eventType)
        {
            case failure_detector::ALIVE:
            {
                membershipUpdateType = JOINED;
                break;
            }

            case failure_detector::FAILED:
            {
                membershipUpdateType = FAILED;
                break;
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported event " << failureDetectorEvent.eventType;
                throw std::logic_error(ss.str());
            }
        }

        onMembershipUpdate(membershipUpdateType, FAILURE_DETECTOR, failureDetectorEvent.address);
    }

    void MembershipProtocol::onGossipEvent(const membership_protocol::MembershipUpdate& membershipUpdate)
    {
        onMembershipUpdate(membershipUpdate.updateType, GOSSIP_PROTOCOL, membershipUpdate.address);
    }

    void MembershipProtocol::onMembershipUpdate(MembershipUpdateType membershipUpdateType, MembershipUpdateSource membershipUpdateSource, const network::Address& sourceAddress)
    {
        if (sourceAddress == node)
        {
            log("Received membership update ", membershipUpdateType, " from ",  membershipUpdateSource, " for current node itself. Ignoring it");
            return;
        }

        auto addressStr = sourceAddress.toString();
        log("Received membership update ", MembershipUpdate::UPDATE_TO_STR[membershipUpdateType], " from ",  MembershipUpdate::SOURCE_TO_STR[membershipUpdateSource], ", node address: ", addressStr);

        switch (membershipUpdateType)
        {
            case membership_protocol::JOINED:
            {
                std::lock_guard<std::mutex> lock(membersMutex);
                bool newNode = members.find(addressStr) == members.end();
                if (newNode)
                {
                    auto result = members.emplace(addressStr, sourceAddress);

                    auto membershipUpdate = MembershipUpdate(result.first->second.address, membership_protocol::JOINED);
                    onMembershipUpdate(membershipUpdate, membershipUpdateSource);
                }
                
                break;
            }

            case membership_protocol::FAILED:
            {
                std::lock_guard<std::mutex> lock(membersMutex);
                bool nodeExists = members.find(addressStr) != members.end();
                if (nodeExists)
                {
                    auto member = members[addressStr];
                    members.erase(addressStr);

                    auto membershipUpdate = MembershipUpdate(member.address, membership_protocol::FAILED);
                    onMembershipUpdate(membershipUpdate, membershipUpdateSource);
                }
                break;
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported membership update " << membershipUpdateType;
                throw std::logic_error(ss.str());
            }
        }
    }
    
    void MembershipProtocol::onMembershipUpdate(const MembershipUpdate& membershipUpdate, MembershipUpdateSource membershipUpdateSource)
    {
        for (auto observer : observers)
        {
            observer->onMembershipUpdate(membershipUpdate);
        }

        if (membershipUpdateSource == FAILURE_DETECTOR || membershipUpdateSource == MEMBERSHIP_PROTOCOL || membershipUpdateSource == INITIAL_SYNC)
        {
            gossipProtocol->spreadMembershipUpdate(membershipUpdate);
        }

        log("Successfully notified observers about membership update");
    }

    void MembershipProtocol::onJoin()
    {
        log("Joined the group");

        failureDetector->start();
        gossipProtocol->start();

        joined = true;
    }

    void MembershipProtocol::processMessage(const std::unique_ptr<Message>& message)
    {
        auto sourceAddress = message->getSourceAddress();
        
        switch (message->getMessageType())
        {
            case JOINREP:
            {
                onJoin();

                auto joinRepMessage = static_cast<membership_protocol::JoinRepMessage*>(message.get());
                gossipProtocol->onNewGossips(joinRepMessage->getSourceAddress(), joinRepMessage->getGossips());

                break; 
            }

            case JOINREQ:
            {
                onMembershipUpdate(JOINED, INITIAL_SYNC, sourceAddress);
                
                auto gossips = gossipProtocol->getGossipsForAddress(sourceAddress);
                messageDispatcher->sendMessage(std::make_unique<JoinRepMessage>(node, sourceAddress, gossips), sourceAddress);

                break;
            }

            case PING:
            {
                auto gossips = gossipProtocol->getGossipsForAddress(sourceAddress);
                messageDispatcher->sendMessage(std::make_unique<AckMessage>(node, sourceAddress, gossips, message->getId()), sourceAddress);

                onMembershipUpdate(JOINED, MEMBERSHIP_PROTOCOL, sourceAddress);
                
                break;
            }

            default:
            {
                log("Unexpected message ", message->getMessageType());
                break;
            }
        }
    }

    network::Address MembershipProtocol::getJoinAddress()
    {
        // TODO: how do we figure out join address?
        return network::Address("1.0.0.0:100");
    }
}
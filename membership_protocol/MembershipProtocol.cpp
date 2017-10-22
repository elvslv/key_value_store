#include <sstream>

#include "MembershipProtocol.h"
#include "MembershipUpdate.h"
#include "JoinReqMessage.h"
#include "JoinRepMessage.h"
#include "AckMessage.h"
#include "../utils/Exceptions.h"


namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::unique_ptr<IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<IGossipProtocolFactory>& gossipProtocolFactory): 
        node(addr),
        messageDispatcher(std::make_shared<utils::MessageDispatcher>(addr, logger)),
        tokens(),
        logger(logger),
        asyncQueue(std::bind(&MembershipProtocol::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<Message> message){asyncQueue.push(std::move(message));}),        
        failureDetector(failureDetectorFactory->createFailureDetector(addr, logger, messageDispatcher, this)),
        gossipProtocol(gossipProtocolFactory->createGossipProtocol(addr, logger, messageDispatcher, this)),
        observers(),
        membersMutex(),
        members(),
        joined(false)
    {
    }

    void MembershipProtocol::start()
    {
        asyncQueue.start();

        failureDetector->addObserver(this);
        failureDetector->start();

        gossipProtocol->addObserver(this);
        gossipProtocol->start();

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
        for (auto it = tokens.begin(); it != tokens.end(); ++it)
        {
            messageDispatcher->stopListening(it->first, it->second);
        }

        // finish processing async queue

        gossipProtocol->stop();
        failureDetector->stop();    

        asyncQueue.stop();
    }

    std::vector<Member> MembershipProtocol::getMembers()
    {
        throw utils::NotImplementedException();
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

    void MembershipProtocol::onGossipEvent(const gossip_protocol::GossipEvent& gossipEvent)
    {
        MembershipUpdateType membershipUpdateType;
        switch (gossipEvent.eventType)
        {
            case gossip_protocol::JOINED:
            {
                membershipUpdateType = JOINED;
                break;
            }

            case gossip_protocol::FAILED:
            {
                membershipUpdateType = FAILED;
                break;
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported event " << gossipEvent.eventType;
                throw std::logic_error(ss.str());
            }
        }

        onMembershipUpdate(membershipUpdateType, GOSSIP_PROTOCOL, gossipEvent.address);
    }

    void MembershipProtocol::onMembershipUpdate(MembershipUpdateType membershipUpdateType, MembershipUpdateSource membershipUpdateSource, const network::Address& sourceAddress)
    {
        if (sourceAddress == node)
        {
            log("Received membership update ", membershipUpdateType, " from ",  membershipUpdateSource, " for current node itself. Ignoring it");
            return;
        }

        auto addressStr = sourceAddress.toString();
        log("Received membership update ", membershipUpdateType, " from ",  membershipUpdateSource, ", node address: ", addressStr);

        switch (membershipUpdateType)
        {
            case membership_protocol::JOINED:
            {
                std::lock_guard<std::mutex> lock(membersMutex);
                bool newNode = members.find(addressStr) == members.end();
                if (newNode)
                {
                    auto result = members.emplace(addressStr, sourceAddress);

                    auto membershipUpdate = MembershipUpdate(result.first->second, membership_protocol::JOINED);
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

                    auto membershipUpdate = MembershipUpdate(member, membership_protocol::FAILED);
                    onMembershipUpdate(membershipUpdate, membershipUpdateSource);
                }
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

        if (membershipUpdateSource == FAILURE_DETECTOR)
        {
            gossipProtocol->spreadMembershipUpdate(membershipUpdate);
        }

        log("Successfully notified observers about membership update");
    }

    void MembershipProtocol::onJoin()
    {
        messageDispatcher->listen(PING, asyncQueueCallback);

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
                break;
            }

            case JOINREQ:
            {
                messageDispatcher->sendMessage(std::make_unique<JoinRepMessage>(node, sourceAddress), sourceAddress);
                
                onMembershipUpdate(JOINED, INITIAL_SYNC, sourceAddress);

                break;
            }

            case PING:
            {
                messageDispatcher->sendMessage(std::make_unique<AckMessage>(node, sourceAddress, message->getId()), sourceAddress);

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
        return node;
    }
}
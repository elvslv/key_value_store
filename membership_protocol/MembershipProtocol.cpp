#include <sstream>

#include "MembershipProtocol.h"
#include "MembershipUpdate.h"
#include "JoinReqMessage.h"
#include "JoinRepMessage.h"
#include "AckMessage.h"


namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::unique_ptr<IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<IGossipProtocolFactory>& gossipProtocolFactory): 
        node(addr),
        messageDispatcher(std::make_shared<utils::MessageDispatcher>(addr, logger)),
        logger(logger),
        failureDetector(failureDetectorFactory->createFailureDetector(addr, logger, messageDispatcher, this)),
        gossipProtocol(gossipProtocolFactory->createGossipProtocol(addr, logger, messageDispatcher, this)),
        observers(),
        membersMutex(),
        members(),
        messagesMutex(),
        messages(),
        joined(false),
        processMessages(false),
        messageProcessingThread()
    {
    }

    void MembershipProtocol::start()
    {
        failureDetector->addObserver(this);
        gossipProtocol->addObserver(this);
        
        messageProcessingThread = std::make_unique<std::thread>(&MembershipProtocol::processMessagesQueue, this);        

        auto targetAddress = getJoinAddress();
        if (targetAddress == node)
        {
            messageDispatcher->listen(JOINREQ, this);
            onJoin();
            return;
        }

        messageDispatcher->listen(JOINREP, this);
        messageDispatcher->sendMessage(std::make_unique<JoinReqMessage>(node, targetAddress), targetAddress);
    }

    void MembershipProtocol::stop()
    {
        gossipProtocol->stop();
        failureDetector->stop();    

        processMessages = false;
        messageProcessingThread->join();
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
                    members[addressStr] = Member(sourceAddress);

                    auto membershipUpdate = MembershipUpdate(members[addressStr], membership_protocol::JOINED);
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
                    members.erase(addressStr);

                    auto membershipUpdate = MembershipUpdate(members[addressStr], membership_protocol::FAILED);
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
        messageDispatcher->listen(PING, this);

        failureDetector->start();
        gossipProtocol->start();

        joined = true;
    }

    void MembershipProtocol::onMessage(const std::unique_ptr<Message>& message)
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push(std::move(message));
    }

    void MembershipProtocol::processMessagesQueue()
    {
        while (processMessages)
        {
            std::unique_ptr<Message> message;
            {
                std::lock_guard<std::mutex> lock(messagesMutex);
                if (!messages.empty())
                {
                    message = std::move(messages.front());
                    messages.pop();
                }
            }

            if (!message)
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                continue;
            }

            processMessage(message);
        }
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
                messageDispatcher->sendMessage(std::make_unique<AckMessage>(node, sourceAddress), sourceAddress);

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
}
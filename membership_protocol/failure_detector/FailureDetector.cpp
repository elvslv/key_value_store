#include "FailureDetector.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"
#include "membership_protocol/messages/AckMessage.h"
#include "membership_protocol/messages/PingMessage.h"

namespace failure_detector
{
    FailureDetector::FailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::unique_ptr<utils::IThreadPolicy>& threadPolicy):
        address(addr),
        logger(logger),
        messageDispatcher(messageDispatcher),
        tokens(),
        membershipProtocol(membershipProtocol),
        gossipProtocol(gossipProtocol),
        threadPolicy(std::move(threadPolicy)),
        observers(),
        asyncQueue(std::bind(&FailureDetector::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<membership_protocol::Message> message){asyncQueue.push(std::move(message));}),
        members(),
        msgIdsMutex(),
        msgIds(),
        runnable([this](){run();})
    {
    }
    
    void FailureDetector::start()
    {
        membershipProtocol->addObserver(this);
        asyncQueue.start();
        
        // tokens[PING_REQ] = messageDispatcher->listen(PING_REQ, asyncQueueCallback);
        tokens[membership_protocol::ACK] = messageDispatcher->listen(membership_protocol::ACK, asyncQueueCallback);
        
        runnable.start();
    }

    void FailureDetector::stop()
    {
        for (auto token : tokens)
        {
            messageDispatcher->stopListening(token.first, token.second);
        }
        asyncQueue.stop();

        runnable.stop();
    }

    void FailureDetector::run()
    {
        logger->log("[FailureDetector::run] -- start");

        while (runnable.shouldRun())
        {
            network::Address address;
            if (!members.getNextElement(address))
            {
                // TODO: add const
                threadPolicy->sleepMilliseconds(100);
                continue;
            }

            sendPing(address);
        }

        logger->log("[FailureDetector::run] -- finish");
    }

    void FailureDetector::sendPing(const network::Address destAddress)
    {
        auto message = std::make_unique<membership_protocol::PingMessage>(address, destAddress, gossipProtocol->getGossipsForAddress(destAddress));
        auto msgId = message->getId();

        {
            std::lock_guard<std::mutex> lock(msgIdsMutex);
            msgIds.insert({msgId, false});
        }

        messageDispatcher->sendMessage(std::move(message), destAddress);
        logger->log("<FailureDetector> -- Successfully sent ping message to ", destAddress.toString(), " message id: ", msgId);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
        bool receivedResponse = false;
        {
            std::lock_guard<std::mutex> lock(msgIdsMutex);
            auto it = msgIds.find(msgId);
            assert(it != msgIds.end());
            receivedResponse = it->second;

            msgIds.erase(it);
        }

        if (receivedResponse)
        {
            logger->log("Received ACK for message ", msgId);
        }
        else
        {
            logger->log("Haven't received ACK for message ", msgId);
        }

        failure_detector::FailureDetectorEvent failureDetectorEvent{ destAddress, receivedResponse ? failure_detector::ALIVE : failure_detector::FAILED };
        for (auto observer : observers)
        {
            observer->onFailureDetectorEvent(failureDetectorEvent);
        }
    }

    void FailureDetector::addObserver(IFailureDetector::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void FailureDetector::onMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate)
    {
        auto address = membershipUpdate.address;
        switch (membershipUpdate.updateType)
        {
            case membership_protocol::JOINED:
            {
                members.insert(address);
                break;
            }
            
            case membership_protocol::FAILED:
            {
                members.remove(address);
                break;
            }
        }
    }

    void FailureDetector::processMessage(const std::unique_ptr<membership_protocol::Message>& message)
    {
        logger->log("<FailureDetector> -- received message ", message->getMessageType(), " from ", message->getSourceAddress());
        switch (message->getMessageType())
        {
            case membership_protocol::ACK:
            {
                auto ackMessage = static_cast<membership_protocol::AckMessage*>(message.get());
                auto msgId = ackMessage->getPingMessageId();
                {
                    std::lock_guard<std::mutex> lock(msgIdsMutex);
                    auto it = msgIds.find(msgId);
                    if (it == msgIds.end())
                    {
                        logger->log("Unexpected ACK message ", ackMessage->getPingMessageId());
                        return;
                    }

                    it->second = true;
                }

                break;
            }

            default:
                logger->log("Unexpected message ", message->toString());
                break;
        }
    }
}
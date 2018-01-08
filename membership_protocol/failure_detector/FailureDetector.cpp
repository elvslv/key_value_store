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
        ackReceivedCondVar(),
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
        runnable.stop();
        asyncQueue.stop();

        for (auto token : tokens)
        {
            messageDispatcher->stopListening(token.first, token.second);
        }
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

            // TODO: add const
            threadPolicy->sleepMilliseconds(2000);
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

        bool receivedResponse = false;
        auto now = std::chrono::steady_clock::now();
        auto expires = std::chrono::seconds(2) + now;
        assert(now != expires);
        while (true)
        {
            std::unique_lock<std::mutex> lock(msgIdsMutex);
            auto res = ackReceivedCondVar.wait_until(lock, expires);

            auto it = msgIds.find(msgId);
            assert(it != msgIds.end());
            receivedResponse = it->second;
            if (receivedResponse || res == std::cv_status::timeout)
            {
                msgIds.erase(it);
                break;
            }
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
                    std::unique_lock<std::mutex> lock(msgIdsMutex);
                    auto it = msgIds.find(msgId);
                    if (it == msgIds.end())
                    {
                        logger->log("Unexpected ACK message ", ackMessage->getPingMessageId());
                        return;
                    }

                    it->second = true;

                    lock.unlock();
                    ackReceivedCondVar.notify_all();
                }

                break;
            }

            default:
                logger->log("Unexpected message ", message->toString());
                break;
        }
    }
}
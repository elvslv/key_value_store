#include "FailureDetector.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"
#include "membership_protocol/messages/PingMessage.h"

namespace failure_detector
{
    FailureDetector::FailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol):
        address(addr),
        logger(logger),
        messageDispatcher(messageDispatcher),
        tokens(),
        membershipProtocol(membershipProtocol),
        observers(),
        asyncQueue(std::bind(&FailureDetector::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<membership_protocol::Message> message){asyncQueue.push(std::move(message));}),
        members(),
        messageProcessingThread(),
        isRunning(false),
        msgIdsMutex(),
        msgIds()
    {
    }
    
    void FailureDetector::start()
    {
        membershipProtocol->addObserver(this);
        
        // tokens[PING_REQ] = messageDispatcher->listen(PING_REQ, asyncQueueCallback);
        tokens[membership_protocol::ACK] = messageDispatcher->listen(membership_protocol::ACK, asyncQueueCallback);
        isRunning = true;        
        messageProcessingThread = std::make_unique<std::thread>(&FailureDetector::run, this);
    }

    void FailureDetector::stop()
    {
        for (auto token : tokens)
        {
            messageDispatcher->stopListening(token.first, token.second);
        }

        isRunning = false;
        messageProcessingThread->join();
    }

    void FailureDetector::run()
    {
        while (isRunning)
        {
            network::Address address;
            if (!members.getNextElement(address))
            {
                // TODO: add const
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                continue;
            }

            sendPing(address);
        }
    }

    void FailureDetector::sendPing(const network::Address destAddress)
    {
        auto message = std::make_unique<membership_protocol::PingMessage>(address, destAddress);
        auto msgId = message->getId();

        {
            std::lock_guard<std::mutex> lock(msgIdsMutex);
            msgIds.insert({msgId, false});
        }

        messageDispatcher->sendMessage(std::move(message), destAddress);
        logger->log("Successfully sent ping message to ", destAddress.toString(), " message id: ", msgId);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(100ms);
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
        switch (message->getMessageType())
        {
            case membership_protocol::ACK:
            {
                auto msgId = message->getId();
                {
                    std::lock_guard<std::mutex> lock(msgIdsMutex);
                    auto it = msgIds.find(msgId);
                    if (it == msgIds.end())
                    {
                        logger->log("Unexpected ACK message ", message->toString());
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
#include "FailureDetector.h"
#include "membership_protocol/messages/AckMessage.h"
#include "membership_protocol/messages/PingMessage.h"
#include "membership_protocol/messages/PingReqMessage.h"
#include "utils/Exceptions.h"
#include "utils/Utils.h"

namespace failure_detector
{
class StopRequestedException : std::exception
{
public:
    StopRequestedException() {}
};

FailureDetector::FailureDetector(const network::Address& addr, std::shared_ptr<utils::Log> logger, std::shared_ptr<utils::MessageDispatcher<membership_protocol::Message>> messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol, std::shared_ptr<utils::IThreadPolicy> threadPolicy)
    : address(addr)
    , logger(logger)
    , messageDispatcher(messageDispatcher)
    , tokens()
    , membershipProtocol(membershipProtocol)
    , gossipProtocol(gossipProtocol)
    , threadPolicy(threadPolicy)
    , observers()
    , asyncQueue(std::bind(&FailureDetector::processMessage, this, std::placeholders::_1))
    , asyncQueueCallback([this](std::unique_ptr<membership_protocol::Message> message) { asyncQueue.push(std::move(message)); })
    , members()
    , msgIdsMutex()
    , ackReceivedCondVar()
    , msgIds()
    , runnable([this]() { run(); })
    , pingReqThreads()
    , stopped(false)
    , stopRequested(false)
{
}

FailureDetector::~FailureDetector()
{
    stop();

    logger->log(address, "~FailureDetector");
}

void FailureDetector::start()
{
    logger->log(address, "[FailureDetector::start]");

    membershipProtocol->addObserver(this);
    asyncQueue.start();

    tokens[membership_protocol::Message::PING_REQ] = messageDispatcher->listen(membership_protocol::Message::PING_REQ, asyncQueueCallback);
    tokens[membership_protocol::Message::ACK] = messageDispatcher->listen(membership_protocol::Message::ACK, asyncQueueCallback);

    runnable.start();
}

void FailureDetector::stop()
{
    if (stopped)
    {
        return;
    }

    runnable.stop();
    asyncQueue.stop();

    for (auto token : tokens)
    {
        messageDispatcher->stopListening(token.first, token.second);
    }

    // TODO: cleanup when thread finishes
    for (auto it = pingReqThreads.begin(); it != pingReqThreads.end(); ++it)
    {
        it->join();
    }

    stopped = true;
    logger->log(address, "[FailureDetector::stop]");
}

void FailureDetector::requestStop()
{
    stopRequested = true;
}

void FailureDetector::run()
{
    logger->log(address, "[FailureDetector::run] -- start");

    while (runnable.isRunning)
    {
        network::Address address;
        if (!members.getNextElement(address))
        {
            // TODO: add const
            threadPolicy->sleepMilliseconds(100);
            continue;
        }

        bool result;
        try
        {
            result = sendPing(address, 1);
        }
        catch (StopRequestedException)
        {
            logger->log(address, "[FailureDetector::run] -- stop requested, exiting");
            return;
        }

        failure_detector::FailureDetectorEvent failureDetectorEvent{ address, result ? failure_detector::ALIVE : failure_detector::FAILED };
        for (auto observer : observers)
        {
            observer->onFailureDetectorEvent(failureDetectorEvent);
        }

        // TODO: add const
        threadPolicy->sleepMilliseconds(1000);
    }

    logger->log(address, "[FailureDetector::run] -- finish");
}

bool FailureDetector::sendMessage(std::unique_ptr<membership_protocol::Message> message, const network::Address& destAddress, int timeoutSeconds)
{
    auto msgId = message->getId();
    {
        std::lock_guard<std::mutex> lock(msgIdsMutex);
        msgIds.insert({ msgId, false });
    }

    messageDispatcher->sendMessage(std::move(message), destAddress);
    logger->log(address, "<FailureDetector> -- Successfully sent ", message->toString());

    bool receivedResponse = false;
    auto now = std::chrono::steady_clock::now();
    auto expires = std::chrono::seconds(timeoutSeconds) + now;
    assert(now != expires);
    while (true)
    {
        std::unique_lock<std::mutex> lock(msgIdsMutex);
        auto res = ackReceivedCondVar.wait_until(lock, expires);

        auto it = msgIds.find(msgId);
        assert(it != msgIds.end());
        receivedResponse = it->second;
        if (stopRequested)
        {
            throw StopRequestedException();
        }

        if (receivedResponse || res == std::cv_status::timeout)
        {
            msgIds.erase(it);
            break;
        }
    }

    if (receivedResponse)
    {
        logger->log(address, "Received ACK for message ", msgId);
    }
    else
    {
        logger->log(address, "Haven't received ACK for message ", msgId);
    }

    return receivedResponse;
}

bool FailureDetector::sendPing(const network::Address& destAddress, int k)
{
    auto message = std::make_unique<membership_protocol::PingMessage>(address, destAddress, gossipProtocol->getGossipsForAddress(destAddress));
    // TODO: figure out timeout
    if (sendMessage(std::move(message), destAddress, 2))
    {
        return true;
    }

    // TODO: add a const with number of hops
    auto otherNodes = members.getKHops(k, destAddress);
    logger->log(address, "Found ", otherNodes.size(), " for ", destAddress);

    for (auto it = otherNodes.begin(); it != otherNodes.end(); ++it)
    {
        if (sendPingReq(*it, destAddress))
        {
            return true;
        }
    }

    return false;
}

bool FailureDetector::sendPingReq(const network::Address& destAddress, const network::Address& target)
{
    auto message = std::make_unique<membership_protocol::PingReqMessage>(address, destAddress, gossipProtocol->getGossipsForAddress(destAddress), target);
    // figure out timeout
    return sendMessage(std::move(message), destAddress, 5);
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

void FailureDetector::onPingReq(std::unique_ptr<membership_protocol::Message> message)
{
    auto pingReqMessage = static_cast<membership_protocol::PingReqMessage*>(message.get());
    auto result = sendPing(pingReqMessage->getTargetAddress(), 0);
    if (result)
    {
        auto sourceAddress = message->getSourceAddress();
        auto gossips = gossipProtocol->getGossipsForAddress(sourceAddress);
        messageDispatcher->sendMessage(std::make_unique<membership_protocol::AckMessage>(address, sourceAddress, gossips, message->getId()), sourceAddress);
    }
}

void FailureDetector::processMessage(std::unique_ptr<membership_protocol::Message> message)
{
    logger->log(address, "<FailureDetector> -- received message ", message->getMessageTypeDescription(), " from ", message->getSourceAddress());
    switch (message->getMessageType())
    {
    case membership_protocol::Message::ACK:
    {
        auto ackMessage = static_cast<membership_protocol::AckMessage*>(message.get());
        auto msgId = ackMessage->getPingMessageId();
        {
            std::unique_lock<std::mutex> lock(msgIdsMutex);
            auto it = msgIds.find(msgId);
            if (it == msgIds.end())
            {
                logger->log(address, "Unexpected ACK message ", ackMessage->getPingMessageId());
                return;
            }

            gossipProtocol->onNewGossips(ackMessage->getSourceAddress(), ackMessage->getGossips());
            it->second = true;

            lock.unlock();
            ackReceivedCondVar.notify_all();
        }

        break;
    }

    case membership_protocol::Message::PING_REQ:
    {
        pingReqThreads.push_back(std::thread(&FailureDetector::onPingReq, this, std::move(message)));
        break;
    }

    default:
        logger->log(address, "Unexpected message ", message->toString());
        break;
    }
}
}
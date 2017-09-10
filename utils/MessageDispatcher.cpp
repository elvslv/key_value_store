#include <sstream>
#include <chrono>

#include "MessageDispatcher.h"

namespace utils
{
    MessageDispatcher::MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger):
        network(address),
        logger(logger),
        listeners(),
        isRunning(false),
        thread()
    {
    }

    void MessageDispatcher::listen(membership_protocol::MsgTypes msgType, IListener* listener)
    {
        if (listeners.find(msgType) != listeners.end())
        {
            std::stringstream ss;
            ss << "Listener to message " << msgType << " already exists";
            throw std::logic_error(ss.str());
        }

        listeners[msgType] = listener;
    }

    void MessageDispatcher::stopListening(membership_protocol::MsgTypes msgType, IListener* listener)
    {
        auto it = listeners.find(msgType);
        if (it == listeners.end())
        {
            std::stringstream ss;
            ss << "Listener to message " << msgType << " not found";
            throw std::logic_error(ss.str());
        }

        if (it->second != listener)
        {
            std::stringstream ss;
            ss << "Given listener " << listener << " of message " <<   msgType <<  " doesn't match registered listener " << it->second;
            throw std::logic_error(ss.str());
        }

        listeners.erase(it);
    }

    void MessageDispatcher::start()
    {
        isRunning = true;
        thread = std::make_unique<std::thread>(&MessageDispatcher::run, this);
    }

    void MessageDispatcher::stop()
    {
        isRunning = false;
        thread->join();
    }

    void MessageDispatcher::run()
    {
        while (isRunning)
        {
            auto message = network.receive();
            if (message.empty())
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                continue;
            }

            auto parsedMessage = membership_protocol::Message::parseMessage(message);
            logger->log("<MessageDispatcher> -- Received msg ", parsedMessage->getMessageType(), " from ", parsedMessage->getSourceAddress());
            auto it = listeners.find(parsedMessage->getMessageType());
            if (it == listeners.end())
            {
                logger->log("<MessageDispatcher> -- listeners of ", parsedMessage->getMessageType(), " not found. Discarding this message");
                continue;
            }

            it->second->onMessage(parsedMessage);
        }
    }

    void MessageDispatcher::sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress)
    {
        auto msg = message->serialize();
        logger->log("Sending message", msg.content.get());

        network->send(destAddress, msg);
    }

}
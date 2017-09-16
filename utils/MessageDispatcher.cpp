#include <sstream>
#include <chrono>
#include <string>

#include "MessageDispatcher.h"
#include "Utils.h"

namespace utils
{
    MessageDispatcher::MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger):
        network(address),
        logger(logger),
        callbacks(),
        isRunning(false),
        thread()
    {
    }

    std::string MessageDispatcher::listen(membership_protocol::MsgTypes msgType, const Callback& callback)
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (callbacks.find(msgType) != callbacks.end())
        {
            std::stringstream ss;
            ss << "Listener to message " << msgType << " already exists";
            throw std::logic_error(ss.str());
        }

        std::string token = Utils::getRandomString(TOKEN_LENGTH);
        callbacks[msgType] = callback;
        tokens[msgType] = token;

        return token;
    }

    void MessageDispatcher::stopListening(membership_protocol::MsgTypes msgType, const std::string& token)
    {
        std::lock_guard<std::mutex> lock(mutex);
        
        auto it = callbacks.find(msgType);
        if (it == callbacks.end())
        {
            std::stringstream ss;
            ss << "Listener to message " << msgType << " not found";
            throw std::logic_error(ss.str());
        }

        if (tokens[msgType] != token)
        {
            std::stringstream ss;
            ss << "Given listener " << token << " of message " <<   msgType <<  " doesn't match registered listener " << tokens[msgType];
            throw std::logic_error(ss.str());
        }

        callbacks.erase(it);
        tokens.erase(msgType);
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
                std::this_thread::sleep_for(SLEEP_DELAY);
                continue;
            }

            auto parsedMessage = membership_protocol::Message::parseMessage(message);
            logger->log("<MessageDispatcher> -- Received msg ", parsedMessage->getMessageType(), " from ", parsedMessage->getSourceAddress().toString());
            
            Callback callback;
            {
                std::lock_guard<std::mutex> lock(mutex);

                auto it = callbacks.find(parsedMessage->getMessageType());
                if (it == callbacks.end())
                {
                    logger->log("<MessageDispatcher> -- listeners of ", parsedMessage->getMessageType(), " not found. Discarding this message");
                    continue;
                }

                callback = it->second;
            }


            callback(parsedMessage);
        }
    }

    void MessageDispatcher::sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress)
    {
        auto msg = message->serialize();
        logger->log("Sending message", msg.content.get());

        network.send(destAddress, msg);
    }
}
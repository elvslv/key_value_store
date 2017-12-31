#include <sstream>
#include <chrono>
#include <string>

#include "MessageDispatcher.h"
#include "Utils.h"

namespace utils
{
    const auto MessageDispatcher::SLEEP_DELAY = std::chrono::milliseconds(100);

    MessageDispatcher::MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger):
        network(address),
        logger(logger),
        callbacks()
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

    void MessageDispatcher::run()
    {
        while (isRunning)
        {
            auto message = network.receive();
            if (message.empty())
            {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(MessageDispatcher::SLEEP_DELAY);
                continue;
            }

            auto parsedMessage = membership_protocol::Message::parseMessage(message);
            logger->log("<MessageDispatcher> -- Received msg ", parsedMessage->getMessageType(), " from ", parsedMessage->getSourceAddress().toString(), " to ", parsedMessage->getDestinationAddress(), " id ", parsedMessage->getId());
            
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


            callback(std::move(parsedMessage));
        }
    }

    void MessageDispatcher::sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress)
    {
        auto msg = message->serialize();
        logger->log("Sending message", msg.content.get(), " to ", destAddress.toString());

        network.send(destAddress, msg);
    }
}
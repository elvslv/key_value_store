#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <chrono>

#include "network/Message.h"
#include "network/Network.h"
#include "membership_protocol/messages/Message.h"
#include "utils/MessageDispatcher.h"
#include "utils/Runnable.h"
#include "Log.h"

namespace utils
{
    class MessageDispatcher: public Runnable
    {
    public:
        typedef std::function<void(std::unique_ptr<membership_protocol::Message>)> Callback;

        MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger);

        std::string listen(membership_protocol::MsgTypes msgType, const Callback& callback);
        void stopListening(membership_protocol::MsgTypes msgType, const std::string& token);
        virtual void sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress);
    private:
        static const int TOKEN_LENGTH = 10;
        static const std::chrono::duration<long long, std::milli> SLEEP_DELAY;

        network::Network network;
        std::shared_ptr<Log> logger;
        std::mutex mutex;
        std::unordered_map<membership_protocol::MsgTypes, Callback> callbacks;
        std::unordered_map<membership_protocol::MsgTypes, std::string> tokens;
        
        virtual void run();
    };
}
#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>
#include <chrono>

#include "../network/Message.h"
#include "../network/Network.h"
#include "../membership_protocol/Message.h"
#include "Log.h"

namespace utils
{
    class MessageDispatcher
    {
    public:
        typedef std::function<void(std::unique_ptr<membership_protocol::Message>&)> Callback;

        MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger);

        std::string listen(membership_protocol::MsgTypes msgType, const Callback& callback);
        void stopListening(membership_protocol::MsgTypes msgType, const std::string& token);
        void start();
        void stop();
        void sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress);
    private:
        static const int TOKEN_LENGTH = 10;
        static constexpr auto SLEEP_DELAY = std::chrono::milliseconds(100);

        network::Network network;
        std::shared_ptr<Log> logger;
        std::mutex mutex;
        std::unordered_map<membership_protocol::MsgTypes, Callback> callbacks;
        std::unordered_map<membership_protocol::MsgTypes, std::string> tokens;
        
        volatile bool isRunning;
        std::unique_ptr<std::thread> thread;

        void run();
    };
}
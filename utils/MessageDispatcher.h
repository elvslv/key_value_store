#pragma once

#include <memory>
#include <thread>
#include <unordered_map>

#include "../network/Message.h"
#include "../network/Network.h"
#include "../membership_protocol/Message.h"
#include "Log.h"

namespace utils
{
    class MessageDispatcher
    {
    public:
        class IListener
        {
        public:
            virtual void onMessage(const std::unique_ptr<membership_protocol::Message>& message) = 0;
        };

        MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger);

        void listen(membership_protocol::MsgTypes msgType, IListener* listener);
        void stopListening(membership_protocol::MsgTypes msgType, IListener* listener);
        void start();
        void stop();
        void sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress);
    private:
        network::Network network;
        std::shared_ptr<Log> logger;
        std::unordered_map<membership_protocol::MsgTypes, IListener*> listeners;

        volatile bool isRunning;
        std::unique_ptr<std::thread> thread;

        void run();
    };
}
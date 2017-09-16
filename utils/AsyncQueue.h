#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include "../membership_protocol/Message.h"

namespace utils
{
    class AsyncQueue
    {
    public:
        typedef std::function<void(std::unique_ptr<membership_protocol::Message>&)> Callback;
        AsyncQueue(const Callback& callback);

        void start();
        void stop();

        void push(std::unique_ptr<membership_protocol::Message>& message);

    private:
        Callback callback;
        std::mutex messagesMutex;
        std::queue<std::unique_ptr<membership_protocol::Message> > messages;
        std::unique_ptr<std::thread> messageProcessingThread;
        
        volatile bool processMessages;

        void processMessagesQueue();        
    };
}
#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <memory>
#include <thread>
#include "membership_protocol/messages/Message.h"
#include "utils/Runnable.h"

namespace utils
{
    class AsyncQueue: public utils::Runnable
    {
    public:
        typedef std::function<void(std::unique_ptr<membership_protocol::Message>)> Callback;
        AsyncQueue(const Callback& callback);

        AsyncQueue(AsyncQueue const&) = delete;
        AsyncQueue& operator =(AsyncQueue const&) = delete;

        void push(std::unique_ptr<membership_protocol::Message> message);

    private:
        Callback callback;
        std::mutex messagesMutex;
        std::queue<std::unique_ptr<membership_protocol::Message> > messages;

        virtual void run();
    };
}
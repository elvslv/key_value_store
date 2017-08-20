#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include "Message.h"

namespace network
{
    class Queue
    {
    public:
        Queue();

        void push(const Message& message);
        Message tryPop();
        bool empty() const;
    private:
        std::queue<Message> queue;
        std::shared_ptr<std::mutex> mutex;
    };
}
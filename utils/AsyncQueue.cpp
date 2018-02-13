#include "AsyncQueue.h"

namespace utils
{
    AsyncQueue::AsyncQueue(const Callback& callback):
        callback(callback),
        messagesMutex(),
        messages()
    {
    }

    AsyncQueue::~AsyncQueue()
    {
        try
        { 
            stop();
        } catch(...) 
        { 
            /*??*/ 
        }
    }

    void AsyncQueue::push(std::unique_ptr<membership_protocol::Message> message)
    {
        std::unique_lock<std::mutex> lock(messagesMutex);
        messages.push(std::move(message));
        lock.unlock();
        condVar.notify_one();
    }

    void AsyncQueue::run()
    {
        while (isRunning)
        {
            std::unique_ptr<membership_protocol::Message> message;
            {
                std::lock_guard<std::mutex> lock(messagesMutex);
                if (!messages.empty())
                {
                    message = std::move(messages.front());
                    messages.pop();
                }
            }

            if (!message)
            {
                using namespace std::chrono_literals;
                std::unique_lock<std::mutex> lock(messagesMutex);
                condVar.wait_for(lock, 2s, [this]{
                    return !messages.empty() || !isRunning;
                });
                continue;
            }

            callback(std::move(message));
        }
    }
}
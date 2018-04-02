#pragma once

#include "utils/Runnable.h"
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

namespace utils
{
template <typename MessageT>
class AsyncQueue : public utils::Runnable
{
public:
    typedef std::function<void(std::unique_ptr<MessageT>)> Callback;
    AsyncQueue(const Callback& callback)
        : callback(callback)
        , messagesMutex()
        , messages()
    {
    }
    virtual ~AsyncQueue()
    {
        try
        {
            stop();
        }
        catch (...)
        {
            /*??*/
        }
    }

    AsyncQueue(AsyncQueue const&) = delete;
    AsyncQueue& operator=(AsyncQueue const&) = delete;

    void push(std::unique_ptr<MessageT> message)
    {
        std::unique_lock<std::mutex> lock(messagesMutex);
        messages.push(std::move(message));
        lock.unlock();
        condVar.notify_one();
    }

private:
    Callback callback;
    std::mutex messagesMutex;
    std::queue<std::unique_ptr<MessageT>> messages;

    virtual void run()
    {
        while (isRunning)
        {
            std::unique_ptr<MessageT> message;
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
                condVar.wait_for(lock, 2s, [this] {
                    return !messages.empty() || !isRunning;
                });
                continue;
            }

            callback(std::move(message));
        }
    }
};
}
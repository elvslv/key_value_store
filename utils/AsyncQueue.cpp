#include "AsyncQueue.h"

namespace utils
{
    AsyncQueue::AsyncQueue(const Callback& callback):
        callback(callback),
        messagesMutex(),
        messages()
    {
    }

    void AsyncQueue::push(std::unique_ptr<membership_protocol::Message> message)
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push(std::move(message));
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
                std::this_thread::sleep_for(100ms);
                continue;
            }

            callback(std::move(message));
        }
    }
}
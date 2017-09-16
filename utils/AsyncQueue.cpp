#include "AsyncQueue.h"

namespace utils
{
    AsyncQueue::AsyncQueue(const Callback& callback):
        callback(callback),
        messagesMutex(),
        messages(),
        messageProcessingThread(),
        processMessages(false)
    {
    }

    void AsyncQueue::start()
    {
        processMessages = true;
        messageProcessingThread = std::make_unique<std::thread>(&AsyncQueue::processMessagesQueue, this);
    }

    void AsyncQueue::stop()
    {
        processMessages = false;
        messageProcessingThread->join();
    }

    void AsyncQueue::push(std::unique_ptr<membership_protocol::Message>& message)
    {
        std::lock_guard<std::mutex> lock(messagesMutex);
        messages.push(std::move(message));
    }

    void AsyncQueue::processMessagesQueue()
    {
        while (processMessages)
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

            callback(message);
        }
    }
}
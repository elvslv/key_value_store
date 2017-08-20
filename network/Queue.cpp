#include "Queue.h"

namespace network
{
    Queue::Queue():
        mutex(new std::mutex)
    {
        
    }

    void Queue::push(const Message& message)
    {
        std::lock_guard<std::mutex> lock(*mutex);
        queue.push(message);
    }

    Message Queue::tryPop()
    {
        std::lock_guard<std::mutex> lock(*mutex);
        if (queue.empty())
        {
            return Message();
        }

        auto result = queue.front();
        queue.pop();

        return result;
    }
}
#include "EmulNet.h"

namespace network
{
    void EmulNet::send(const Address& srcAddr, const Address& destAddr, const Message& message)
    {
        auto key = destAddr.toString();
        Queue& queue = getQueue(key);
        queue.push(message);
    }

    Message EmulNet::receive(const Address& srcAddr)
    {
        auto key = srcAddr.toString();
        Queue& queue = getQueue(key);
        return queue.tryPop();
    }

    Queue& EmulNet::getQueue(const std::string& key)
    {
        std::lock_guard<std::mutex> lock(mutex);
        auto it = queues.find(key);
        if (it == queues.end())
        {
            queues[key] = Queue();
            it = queues.find(key);
        }

        return it->second;
    }
}
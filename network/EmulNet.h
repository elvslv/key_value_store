#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include "Address.h"
#include "Message.h"
#include "Queue.h"

namespace network
{
    class EmulNet
    {
    public:
        EmulNet(EmulNet const&) = delete;
        EmulNet& operator=(EmulNet const&) = delete;

        static std::shared_ptr<EmulNet> instance()
        {
            static std::shared_ptr<EmulNet> s{new EmulNet};
            return s;
        }
        void send(const Address& srcAddr, const Address& destAddr, const Message& message);
        Message receive(const Address& srcAddr);
    private:
        EmulNet() {}
        Queue& getQueue(const std::string& key);

        std::unordered_map<std::string, Queue> queues;
        std::mutex mutex;
    };
}

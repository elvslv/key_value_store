#pragma once

#include <chrono>
#include <functional>
#include <memory>
#include <thread>
#include <unordered_map>

#include "Log.h"
#include "network/Message.h"
#include "network/Network.h"
#include "utils/Message.h"
#include "utils/Runnable.h"
#include "utils/Utils.h"

namespace utils
{
class MessageDispatcher : public Runnable
{
public:
    typedef std::function<void(std::unique_ptr<Message>)> Callback;

    MessageDispatcher(const network::Address& address, const std::shared_ptr<Log>& logger);

    virtual ~MessageDispatcher(){}
    virtual std::string listen(const std::string& msgType, const Callback& callback);
    virtual void stopListening(const std::string& token);

    virtual void sendMessage(std::unique_ptr<Message> message, const network::Address& destAddress);
    virtual void run();

private:
    static const int TOKEN_LENGTH = 10;
    static constexpr std::chrono::duration<long long, std::milli> SLEEP_DELAY = std::chrono::milliseconds(100);

    network::Network network;
    std::shared_ptr<Log> logger;
    std::mutex mutex;
    std::unordered_map<std::string, Callback> callbacks;
    std::unordered_map<std::string, std::string> typeToToken;
    std::unordered_map<std::string, std::string> tokenToType;
};

}
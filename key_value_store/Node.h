#pragma once

#include "CreateRequestMessage.h"
#include "CreateResponseMessage.h"
#include "DeleteRequestMessage.h"
#include "DeleteResponseMessage.h"
#include "INode.h"
#include "IPartitioner.h"
#include "IStorage.h"
#include "ReadRequestMessage.h"
#include "ReadResponseMessage.h"
#include "RequestMessage.h"
#include "ResponseMessage.h"
#include "UpdateRequestMessage.h"
#include "UpdateResponseMessage.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "network/Address.h"
#include "utils/AsyncQueue.h"
#include "utils/Exceptions.h"
#include "utils/IThreadPolicy.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"
#include <array>
#include <exception>
#include <functional>
#include <future>
#include <list>
#include <thread>

namespace key_value_store
{
class Node : public INode
{
public:
    Node(const network::Address& address, std::shared_ptr<utils::Log> logger, std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol, std::unique_ptr<IStorage> storage, std::unique_ptr<IPartitioner> partitioner, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, std::shared_ptr<utils::IThreadPolicy> threadPolicy);

    virtual ~Node() {}

    virtual void start();
    virtual void stop();

    virtual void create(const std::string& key, const std::string& value);
    virtual std::string read(const std::string& key);
    virtual void update(const std::string& key, const std::string& value);
    virtual void remove(const std::string& key);

private:
    struct MessageState
    {
        MessageState()
            : conditionVariable()
            , responseMessage()
        {
        }

        std::condition_variable conditionVariable;
        std::unique_ptr<ResponseMessage> responseMessage;
    };

    void run();

    bool createLocally(const std::string& key, const std::string& value);
    bool sendCreateMessage(const network::Address& target, const std::string& key, const std::string& value);

    Record readLocally(const std::string& key);
    Record sendReadMessage(const network::Address& target, const std::string& key);

    bool updateLocally(const std::string& key, const std::string& value);
    bool sendUpdateMessage(const network::Address& target, const std::string& key, const std::string& value);

    bool removeLocally(const std::string& key);
    bool sendRemoveMessage(const network::Address& target, const std::string& key);

    std::vector<network::Address> getTargetNodes(const std::string& key);

    template <typename ResultT>
    ResultT performActionAndWait(std::function<ResultT()> func, std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv)
    {
        ResultT result;
        try
        {
            result = func();
        }
        catch (...)
        {
            ++count;
            cv.notify_one();
            throw;
        }

        ++succeeded;
        ++count;
        cv.notify_one();
        return result;
    }

    template <typename ResultT>
    ResultT processClientRequest(const std::string& key, std::function<ResultT()> localFunc, std::function<ResultT(const network::Address&)> remoteFunc)
    {
        auto nodes = getTargetNodes(key);
        if (nodes.size() < 3)
        {
            throw std::logic_error("Not enough nodes!!!");
        }

        std::atomic<int> count(0);
        std::atomic<int> succeeded(0);

        std::mutex mutex;
        std::condition_variable cv;

        std::array<std::future<ResultT>, 3> futures;
        for (int i = 0; i < nodes.size(); ++i)
        {
            auto node = nodes[i];
            if (node == address)
            {
                futures[i] = std::async(std::launch::async, [this, localFunc](std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv) {
                    return performActionAndWait<ResultT>([this, localFunc]() -> ResultT { return localFunc(); }, count, succeeded, cv);
                },
                    std::ref(count), std::ref(succeeded), std::ref(cv));
            }
            else
            {
                futures[i] = std::async(std::launch::async, [this, remoteFunc, node](std::atomic<int>& count, std::atomic<int>& succeeded, std::condition_variable& cv) {
                    return performActionAndWait<ResultT>([this, remoteFunc, node]() -> ResultT { return remoteFunc(node); }, count, succeeded, cv);
                },
                    std::ref(count), std::ref(succeeded), std::ref(cv));
            }
        }

        {
            using namespace std::chrono_literals;
            std::unique_lock<std::mutex> lock(mutex);

            // TODO: add a const
            cv.wait_for(lock, 2s, [&count, &succeeded]() -> bool { return count == 3 || succeeded == 2; });
        }

        ResultT result;
        std::unique_ptr<utils::ComplexException> eptr = std::make_unique<utils::ComplexException>();
        for (auto it = futures.begin(); it != futures.end(); ++it)
        {
            try
            {
                result = it->get();
            }
            catch (...)
            {
                eptr->addException(std::current_exception());
            }
        }

        if (succeeded < 2)
        {
            auto ex = eptr.release();
            throw ex;
        }

        return result;
    }

    template <class Rep, class Period>
    std::unique_ptr<ResponseMessage> sendMessageAndWait(std::unique_ptr<RequestMessage> message, const std::chrono::duration<Rep, Period>& timeout)
    {
        auto messageId = message->getId();
        std::unique_lock<std::mutex> lock(mutex);
        assert(sentMessages.find(messageId) == sentMessages.end());

        MessageState messageState;
        sentMessages[messageId] = &messageState;
        auto destAddress = message->getDestinationAddress();

        logger->log("sending ", message->getMsgTypeStr(), "from ", address, "to ", destAddress, message->getId(), message->getKey());
        messageDispatcher->sendMessage(std::move(message), destAddress);
        logger->log("sent from ", address, "to ", destAddress);

        bool result = messageState.conditionVariable.wait_for(lock, timeout, [&messageState] {
            return (bool)messageState.responseMessage;
        });

        if (!result)
        {
            logger->log(address, "failed waiting for response from", destAddress);
        }

        sentMessages.erase(messageId);
        return std::move(messageState.responseMessage);
    }

    void processMessage(std::unique_ptr<Message> message);
    void onCreateRequest(std::unique_ptr<CreateRequestMessage> message);
    void onUpdateRequest(std::unique_ptr<UpdateRequestMessage> message);
    void onReadRequest(std::unique_ptr<ReadRequestMessage> message);
    void onDeleteRequest(std::unique_ptr<DeleteRequestMessage> message);
    void onResponse(std::unique_ptr<ResponseMessage> message);

    void runStabilizationProtocol();
    bool isNodeAlive(const network::Address& addr);

    network::Address address;
    std::shared_ptr<utils::Log> logger;
    std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol;
    std::unique_ptr<IStorage> storage;
    std::unique_ptr<IPartitioner> partitioner;
    std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
    utils::AsyncQueue<Message> asyncQueue;
    utils::AsyncQueue<Message>::Callback asyncQueueCallback;
    std::map<std::string, MessageState*> sentMessages;
    std::mutex mutex;

    utils::RunnableCallback runnable;
    std::shared_ptr<utils::IThreadPolicy> threadPolicy;
};
}
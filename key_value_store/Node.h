#pragma once

#include <list>
#include "INode.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "network/Address.h"
#include "utils/Log.h"
#include "utils/Runnable.h"
#include "IStorage.h"
#include "IPartitioner.h"

namespace key_value_store
{
    class Node: INode
    {
    public:
        Node(const network::Address& address, const std::shared_ptr<utils::Log>& logger, std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol, std::unique_ptr<IStorage> storage, std::unique_ptr<IPartitioner> partitioner);

        virtual ~Node(){}

        virtual void start();
        virtual void stop();

        virtual void create(const std::string& key, const std::string& value) = 0;
        virtual std::string read(const std::string& key) = 0;
        virtual void update(const std::string& key, const std::string& value) = 0;
        virtual void remove(const std::string& key) = 0;

    private:
        void run();
        std::list<network::Address> getTargetNodes(const std::string& key);

        network::Address address;
        std::shared_ptr<utils::Log> logger;
        std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol;
        std::unique_ptr<IStorage> storage;
        std::unique_ptr<IPartitioner> partitioner;

        utils::RunnableCallback runnable;
    };
}
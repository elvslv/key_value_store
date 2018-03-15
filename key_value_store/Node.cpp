#include <functional>
#include "Node.h"
#include "utils/Exceptions.h"

namespace key_value_store
{
    Node::Node(const network::Address& address, const std::shared_ptr<utils::Log>& logger, std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol, std::unique_ptr<IStorage> storage, std::unique_ptr<IPartitioner> partitioner):
        address(address),
        logger(logger), 
        membershipProtocol(std::move(membershipProtocol)),
        storage(std::move(storage)),
        partitioner(std::move(partitioner)),
        runnable([this](){run();})
    {
    }

    void Node::start()
    {
        logger->log(address, "Starting node");
        membershipProtocol->start();

        runnable.start();
    }

    void Node::stop()
    {
        runnable.stop();
        membershipProtocol->stop();

        logger->log(address, "Stopped node");
    }

    void Node::create(const std::string& key, const std::string& value)
    {
        throw utils::NotImplementedException();
    }

    std::string Node::read(const std::string& key)
    {
        throw utils::NotImplementedException();
    }

    void Node::update(const std::string& key, const std::string& value)
    {
        throw utils::NotImplementedException();
    }

    void Node::remove(const std::string& key)
    {
        throw utils::NotImplementedException();
    }

    void Node::run()
    {
        logger->log(address, "[Node::run] -- start");

        while (runnable.isRunning)
        {
        }

        logger->log(address, "[Node::run] -- end");
    }

    std::list<network::Address> Node::getTargetNodes(const std::string& key)
    {
        auto nodes = membershipProtocol->getMembers();
        nodes.push_back(membership_protocol::Member(address));

        return partitioner->getTargetNodes(key, nodes);
    }
}
#include "key_value_store/Exceptions.h"
#include "key_value_store/Node.h"
#include "key_value_store/Partitioner.h"
#include "key_value_store/Storage.h"

#include "membership_protocol/MembershipProtocol.h"
#include "membership_protocol/failure_detector/FailureDetectorFactory.h"
#include "membership_protocol/gossip_protocol/GossipProtocolFactory.h"

#include "utils/ThreadPolicy.h"

#include "gtest/gtest.h"

namespace
{
using namespace std::chrono_literals;

std::unique_ptr<key_value_store::Node> createNode(int nodeId, std::shared_ptr<utils::Log> logger)
{
    std::stringstream ss;
    ss << nodeId << ".0.0.0:" << nodeId * 100;
    network::Address addr(ss.str());

    size_t ringSize = 512;
    std::unique_ptr<key_value_store::IPartitioner> partitioner = std::make_unique<key_value_store::Partitioner>(ringSize);

    auto membershipProtocolMessageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);

    std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<failure_detector::FailureDetectorFactory>();
    std::unique_ptr<gossip_protocol::IGossipProtocolFactory> gossipProtocolFactory = std::make_unique<gossip_protocol::GossipProtocolFactory>();

    auto threadPolicy = std::make_shared<utils::ThreadPolicy>();

    std::unique_ptr<membership_protocol::IMembershipProtocol> membershipProtocol = std::make_unique<membership_protocol::MembershipProtocol>(addr, logger, membership_protocol::Config(1), membershipProtocolMessageDispatcher, failureDetectorFactory, gossipProtocolFactory, threadPolicy);

    std::unique_ptr<key_value_store::IStorage> storage = std::make_unique<key_value_store::Storage>();

    return std::make_unique<key_value_store::Node>(addr, logger, std::move(membershipProtocol), std::move(storage), std::move(partitioner), membershipProtocolMessageDispatcher, threadPolicy);
}

TEST(NodeTests, Constructor)
{
    auto logger = std::make_shared<utils::Log>();
    auto node = createNode(1, logger);
}

TEST(NodeTests, ReadNotExists)
{
    auto logger = std::make_shared<utils::Log>();

    std::vector<std::unique_ptr<key_value_store::Node>> nodes;
    for (int i = 1; i <= 3; ++i)
    {
        auto node = createNode(i, logger);
        node->start();
        nodes.push_back(std::move(node));
    }

    std::this_thread::sleep_for(5s);

    bool exceptionCaught = false;
    try
    {
        auto val = nodes[0]->read("key");
    }
    catch (const utils::ComplexException& ex)
    {
        exceptionCaught = true;
    }

    ASSERT_TRUE(exceptionCaught);

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        (*it)->stop();
    }
}

TEST(NodeTests, ReadExists)
{
    auto logger = std::make_shared<utils::Log>();

    std::vector<std::unique_ptr<key_value_store::Node>> nodes;
    for (int i = 1; i <= 3; ++i)
    {
        auto node = createNode(i, logger);
        node->start();
        nodes.push_back(std::move(node));
    }

    std::this_thread::sleep_for(5s);

    std::string key = "key";
    std::string val = "value";
    nodes[1]->create(key, val);

    auto v = nodes[1]->read(key);
    ASSERT_EQ(v, val);

    v = nodes[0]->read(key);
    ASSERT_EQ(v, val);

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        (*it)->stop();
    }
}

TEST(NodeTests, Update)
{
    auto logger = std::make_shared<utils::Log>();

    std::vector<std::unique_ptr<key_value_store::Node>> nodes;
    for (int i = 1; i <= 3; ++i)
    {
        auto node = createNode(i, logger);
        node->start();
        nodes.push_back(std::move(node));
    }

    std::this_thread::sleep_for(5s);

    std::string key = "key";
    std::string val = "value";
    nodes[1]->create(key, val);

    auto v = nodes[1]->read(key);
    ASSERT_EQ(v, val);

    val = "value1";
    nodes[2]->update(key, val);
    v = nodes[0]->read(key);
    ASSERT_EQ(v, val);

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        (*it)->stop();
    }
}

TEST(NodeTests, Remove)
{
    auto logger = std::make_shared<utils::Log>();

    std::vector<std::unique_ptr<key_value_store::Node>> nodes;
    for (int i = 1; i <= 3; ++i)
    {
        auto node = createNode(i, logger);
        node->start();
        nodes.push_back(std::move(node));
    }

    std::this_thread::sleep_for(5s);

    std::string key = "key";
    std::string val = "value";
    nodes[1]->create(key, val);

    auto v = nodes[1]->read(key);
    ASSERT_EQ(v, val);

    nodes[2]->remove(key);

    bool exceptionCaught = false;
    try
    {
        nodes[0]->read(key);
    }
    catch (const utils::ComplexException& ex)
    {
        exceptionCaught = true;
    }

    ASSERT_TRUE(exceptionCaught);

    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it)
    {
        (*it)->stop();
    }
}
}
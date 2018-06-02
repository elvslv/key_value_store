#include "key_value_store/Partitioner.h"
#include "gtest/gtest.h"
#include <sstream>

namespace
{
std::vector<membership_protocol::Member> generateMembers(int n, size_t ringSize)
{
    std::vector<membership_protocol::Member> members;
    for (int i = 1; i <= n; ++i)
    {
        std::stringstream ss;
        ss << i << ".0.0.0:" << i * 100;
        members.push_back(membership_protocol::Member(network::Address(ss.str())));
    }

    std::sort(members.begin(), members.end(), [ringSize](const membership_protocol::Member& member1, const membership_protocol::Member& member2) {
        return member1.hash(ringSize) < member2.hash(ringSize);
    });

    return members;
}

TEST(PartitionerTests, OneNode)
{
    size_t ringSize = 512;
    key_value_store::Partitioner partitioner(ringSize);

    auto members = generateMembers(1, ringSize);

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 1);
    ASSERT_EQ(addresses[0], members[0].address);
}

TEST(PartitionerTests, TwoNodes)
{
    size_t ringSize = 512;
    key_value_store::Partitioner partitioner(ringSize);

    auto members = generateMembers(2, ringSize);

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 2);
    ASSERT_EQ(addresses[0], members[1].address);
    ASSERT_EQ(addresses[1], members[0].address);

    key = "keysdfgdgfdh";
    addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 2);
    ASSERT_EQ(addresses[0], members[1].address);
    ASSERT_EQ(addresses[1], members[0].address);
}

TEST(PartitionerTests, ThreeNodes)
{
    size_t ringSize = 512;
    key_value_store::Partitioner partitioner(ringSize);

    auto members = generateMembers(3, ringSize);

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 3);
    ASSERT_EQ(addresses[0], members[2].address);
    ASSERT_EQ(addresses[1], members[0].address);
    ASSERT_EQ(addresses[2], members[1].address);
}

TEST(PartitionerTests, TenNodes)
{
    size_t ringSize = 512;
    key_value_store::Partitioner partitioner(ringSize);

    auto members = generateMembers(10, ringSize);

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 3);
    ASSERT_EQ(addresses[0], members[6].address);
    ASSERT_EQ(addresses[1], members[7].address);
    ASSERT_EQ(addresses[2], members[8].address);
}
}

#include "key_value_store/Partitioner.h"
#include "gtest/gtest.h"

namespace
{
TEST(PartitionerTests, OneNode)
{
    size_t ringSize = 8;
    key_value_store::Partitioner partitioner(ringSize);

    std::vector<membership_protocol::Member> members{ membership_protocol::Member(network::Address("1.0.0.0:100")) };

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 1);
    ASSERT_EQ(addresses[0], members[0].address);
}

TEST(PartitionerTests, TwoNodes)
{
    size_t ringSize = 8;
    key_value_store::Partitioner partitioner(ringSize);

    std::vector<membership_protocol::Member> members{
        membership_protocol::Member(network::Address("1.0.0.0:100")),
        membership_protocol::Member(network::Address("2.0.0.0:200")),
    };

    std::string key = "key";
    auto addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 2);
    ASSERT_EQ(addresses[0], members[0].address);
    ASSERT_EQ(addresses[1], members[1].address);

    key = "keysdfgdgfdh";
    addresses = partitioner.getTargetNodes(key, members);
    ASSERT_EQ(addresses.size(), 2);
    ASSERT_EQ(addresses[0], members[1].address);
    ASSERT_EQ(addresses[1], members[0].address);
}
}

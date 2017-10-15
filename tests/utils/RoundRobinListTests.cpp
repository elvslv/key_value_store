#include <unordered_set>

#include "gtest/gtest.h"
#include "../../network/Address.h"
#include "../../utils/RoundRobinList.h"

namespace
{
    TEST(RoundRobinListTests, Integers)
    {
        utils::RoundRobinList<int> list;

        int i;
        ASSERT_FALSE(list.getNextElement(i));
        ASSERT_FALSE(list.remove(0));

        int num = 10;        
        for (i = 0; i < num; ++i)
        {
            ASSERT_TRUE(list.insert(i));
        }

        std::unordered_set<int> values;
        for (i = 0; i < num; ++i)
        {
            int val;
            ASSERT_TRUE(list.getNextElement(val));   
            ASSERT_EQ(values.find(val), values.end());
            values.insert(val);

            ASSERT_TRUE(list.remove(val));
        }

        ASSERT_EQ(values.size(), num);
    }

    TEST(RoundRobinListTests, AddressBasic)
    {
        utils::RoundRobinList<network::Address> list;

        network::Address address;
        ASSERT_FALSE(list.getNextElement(address));

        address = network::Address("1.0.0.0:100");
        ASSERT_FALSE(list.remove(address));

        ASSERT_TRUE(list.insert(address));
        ASSERT_FALSE(list.insert(address));
        ASSERT_TRUE(list.remove(address));
        ASSERT_TRUE(list.insert(address));        
    }

    TEST(RoundRobinListTests, AddressRand)
    {
        unsigned char num = 5;
        std::vector<network::Address> addresses;
        addresses.reserve(num);
        for (unsigned char i = 0; i < num; ++i)
        {
            addresses.push_back({{{i, 0, 0, 0}}, i});
        }

        std::unordered_set<network::Address> set;
        utils::RoundRobinList<network::Address> list;
        int repNum = 20;
        for (int i = 0; i < repNum; ++i)
        {
            int j = std::rand() % num;
            auto it = set.find(addresses[j]);
            bool exists = it != set.end();
            bool remove = std::rand() % 2;
            if (remove)
            {
                if (exists)
                {
                    ASSERT_TRUE(list.remove(addresses[j]));
                    set.erase(addresses[j]);
                }
                else
                {
                    ASSERT_FALSE(list.remove(addresses[j]));
                }
            }
            else
            {
                if (!exists)
                {
                    ASSERT_TRUE(list.insert(addresses[j]));
                    set.insert(addresses[j]);
                }
                else
                {
                    ASSERT_FALSE(list.insert(addresses[j]));
                }
            }
        }

        int cnt = set.size();
        for (int i = 0; i < cnt; ++i)
        {
            network::Address address;
            ASSERT_TRUE(list.getNextElement(address));

            auto it = set.find(address);
            ASSERT_NE(it, set.end());
            set.erase(it);

            ASSERT_TRUE(list.remove(address));
        }

        ASSERT_TRUE(set.empty());
    }
}
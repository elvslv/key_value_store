#include <unordered_set>

#include "gtest/gtest.h"
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
}
#include "gtest/gtest.h"
#include "network/Address.h"

namespace 
{
    TEST(AddressTests, Basic)
    {
        std::string addressStr = "1.0.0.0:100";
        network::Address address(addressStr);
        ASSERT_EQ(address.address[0], 1);

        ASSERT_EQ(address.toString(), addressStr);
    }

    TEST(AddressTests, SerializeDeserialize)
    {
        std::string networkAddressStr = "1.0.0.0:100";
        network::Address networkAddress(networkAddressStr);

        auto genAddress = networkAddress.serialize();

        network::Address anotherNetworkAddress(*genAddress.get());

        ASSERT_EQ(networkAddress, anotherNetworkAddress);
        ASSERT_EQ(networkAddressStr, anotherNetworkAddress.toString());
    }
}
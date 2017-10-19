#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "../../membership_protocol/FailureDetector.h"

namespace 
{
    class MockIMembershipProtocol : public membership_protocol::IMembershipProtocol 
    {
    public:
        MOCK_METHOD0(start, void());
        MOCK_METHOD0(stop, void());
        MOCK_METHOD0(getMembers,  std::vector<membership_protocol::Member>());
        MOCK_METHOD1(addObserver, void(membership_protocol::IMembershipProtocol::IObserver* observer));
   };
    
    TEST(FailureDetectorTests, Constructor)
    {
        // FailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, IMembershipProtocol* membershipProtocol)
    
        network::Address addr("1.0.0.0:100");
        auto logger = std::make_shared<utils::Log>();
        auto messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
        MockIMembershipProtocol membershipProtocol;
        membership_protocol::FailureDetector failureDectector(addr, logger, messageDispatcher, &membershipProtocol);
    }
}
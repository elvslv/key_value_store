#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "membership_protocol/gossip_protocol/IGossipProtocol.h"
#include "membership_protocol/failure_detector/IFailureDetector.h"
#include "utils/IThreadPolicy.h"

namespace mock
{
    class MockIMembershipProtocol : public membership_protocol::IMembershipProtocol {
    public:
        MOCK_METHOD0(start,
            void());
        MOCK_METHOD0(stop,
            void());
        MOCK_METHOD0(requestStop,
            void());
        MOCK_METHOD0(getMembers,
            std::vector<membership_protocol::Member>());
        MOCK_METHOD1(addObserver,
            void(IMembershipProtocol::IObserver* observer));
        MOCK_METHOD0(getMembersNum,
            size_t());
    };

    class MockIGossipProtocol : public gossip_protocol::IGossipProtocol {
    public:
        MOCK_METHOD0(start,
            void());
        MOCK_METHOD0(stop,
            void());
        MOCK_METHOD1(addObserver,
            void(IObserver* observer));
        MOCK_METHOD1(spreadMembershipUpdate,
            void(const membership_protocol::MembershipUpdate& membershipUpdate));
        MOCK_METHOD1(getGossipsForAddress,
            std::vector<membership_protocol::Gossip>(const network::Address& address));
        MOCK_METHOD2(onNewGossips,
            void(const network::Address& sourceAddress, const std::vector<membership_protocol::Gossip>& gossips));
    };

    class MockIFailureDetector : public failure_detector::IFailureDetector {
    public:
        MOCK_METHOD0(start,
            void());
        MOCK_METHOD0(stop,
            void());
        MOCK_METHOD0(requestStop,
            void());
        MOCK_METHOD1(addObserver,
            void(IObserver* observer));
    };

    class MockIThreadPolicy : public utils::IThreadPolicy {
    public:
        MOCK_CONST_METHOD1(sleepMilliseconds,
            void(unsigned int milliseconds));
    };
}
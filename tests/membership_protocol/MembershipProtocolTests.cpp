#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../mocks/Mocks.h"

#include "membership_protocol/MembershipProtocol.h"
#include "membership_protocol/gossip_protocol/GossipProtocol.h"
#include "membership_protocol/failure_detector/FailureDetector.h"
#include "utils/ThreadPolicy.h"
#include "utils/Utils.h"


namespace
{
    using namespace std::chrono_literals;

    class FakeFailureDetectorFactory: public failure_detector::IFailureDetectorFactory
    {
        virtual std::unique_ptr<failure_detector::IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol)
        {
            return std::make_unique<testing::NiceMock<mock::MockIFailureDetector> >();
        }
    };

    class FakeGossipProtocolFactory: public gossip_protocol::IGossipProtocolFactory
    {
        virtual std::unique_ptr<gossip_protocol::IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            return std::make_unique<testing::NiceMock<mock::MockIGossipProtocol> >();
        }
    };

    class FailureDetectorFactory: public failure_detector::IFailureDetectorFactory
    {
        virtual std::unique_ptr<failure_detector::IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol, gossip_protocol::IGossipProtocol* gossipProtocol)
        {
            std::unique_ptr<utils::IThreadPolicy> threadPolicy = std::make_unique<utils::ThreadPolicy>();
            return std::make_unique<failure_detector::FailureDetector>(addr, logger, messageDispatcher, membershipProtocol, gossipProtocol, threadPolicy);
        }
    }; 

    class GossipProtocolFactory: public gossip_protocol::IGossipProtocolFactory
    {
        virtual std::unique_ptr<gossip_protocol::IGossipProtocol> createGossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol)
        {
            std::unique_ptr<utils::IThreadPolicy> threadPolicy = std::make_unique<utils::ThreadPolicy>();
            return std::make_unique<gossip_protocol::GossipProtocol>(addr, logger, membershipProtocol, threadPolicy);
        }
    };

    class FakeMessageDispatcher: public utils::MessageDispatcher
    {
    public:
        FakeMessageDispatcher(const network::Address& address, const std::shared_ptr<utils::Log>& logger):
            utils::MessageDispatcher(address, logger)
        {
        }

        virtual void sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress)
        {
            if (message->getMessageType() == membership_protocol::ACK || message->getMessageType() == membership_protocol::PING || message->getMessageType() == membership_protocol::JOINREQ)
            {
                return;
            }

            utils::MessageDispatcher::sendMessage(message, destAddress);
        } 

        virtual std::string listen(membership_protocol::MsgTypes msgType, const Callback& callback)
        {
            return utils::Utils::getRandomString(8);
        }

        virtual void stopListening(membership_protocol::MsgTypes msgType, const std::string& token)
        {
            return;
        }
    };

    class BrokenLinkMessageDispatcher: public utils::MessageDispatcher
    {
    public:
        BrokenLinkMessageDispatcher(const network::Address& address, const network::Address& otherAddress, const std::shared_ptr<utils::Log>& logger):
            utils::MessageDispatcher(address, logger),
            otherAddress(otherAddress)
        {
        }

        virtual void sendMessage(const std::unique_ptr<membership_protocol::Message>& message, const network::Address& destAddress)
        {
            if (destAddress == otherAddress)
            {
                return;
            }

            utils::MessageDispatcher::sendMessage(message, destAddress);
        }

    private:
        network::Address otherAddress;
    };


    template <class Rep, class Period>
    void test_n_nodes(unsigned char n, const std::chrono::duration<Rep, Period>& timeout)
    {
        auto logger = std::make_shared<utils::Log>();
        std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
        std::unique_ptr<gossip_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
        
        std::vector<std::unique_ptr<membership_protocol::MembershipProtocol>> membershipProtocols;
        for (unsigned char i = 0; i < n; ++i)
        {
            network::Address addr(std::array<unsigned char, 4>{{static_cast<unsigned char>(i + 1), 0, 0, 0}}, (i + 1) * 100);
            std::shared_ptr<utils::MessageDispatcher> messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            membershipProtocols.push_back(std::make_unique<membership_protocol::MembershipProtocol>(addr, logger, membership_protocol::Config(1), messageDispatcher, failureDetectorFactory, goossipProtocolFactory));
        }

        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it)
        {
            (*it)->start();
        }

        // TODO: replace sleep with wait
        std::this_thread::sleep_for(timeout);

        int i = 0;
        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it, ++i)
        {
            auto members = (*it)->getMembers();
            ASSERT_FALSE(members.empty());
            auto membersNum = (*it)->getMembersNum();
            ASSERT_EQ(members.size(), membersNum);
            ASSERT_EQ(membersNum, n - 1);
        }

        for (auto it = membershipProtocols.rbegin(); it != membershipProtocols.rend(); ++it)
        {
            (*it)->stop();
        }
    }

    TEST(MembershipProtocolTests, Consructor)
    {
        network::Address addr("1.0.0.0:100");
        auto logger = std::make_shared<utils::Log>();
        std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
        std::unique_ptr<gossip_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
        std::shared_ptr<utils::MessageDispatcher> messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);

        membership_protocol::MembershipProtocol membershipProtocol(addr, logger,  membership_protocol::Config(1), messageDispatcher, failureDetectorFactory, goossipProtocolFactory);
        membershipProtocol.start();
        auto members = membershipProtocol.getMembers();
        ASSERT_TRUE(members.empty());
        ASSERT_EQ(membershipProtocol.getMembersNum(), 0);
        membershipProtocol.stop();
    }

    TEST(MembershipProtocolTests, TwoNodes)
    {
        test_n_nodes(2, 5s);
    }

    TEST(MembershipProtocolTests, ThreeNodes)
    {
        test_n_nodes(3, 7s);
    }

    template <class Rep, class Period>
    void test_n_nodes_one_failed(int n, const std::chrono::duration<Rep, Period>& timeout)
    {
        auto logger = std::make_shared<utils::Log>();
        std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
        std::unique_ptr<gossip_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
        
        std::vector<std::unique_ptr<membership_protocol::MembershipProtocol>> membershipProtocols;
        for (unsigned char i = 0; i < n; ++i)
        {
            network::Address addr(std::array<unsigned char, 4>{{static_cast<unsigned char>(i + 1), 0, 0, 0}}, (i + 1) * 100);
            std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
            if (i < n - 1)
            {
                messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            }
            else
            {
                messageDispatcher = std::make_shared<FakeMessageDispatcher>(addr, logger);
            }
            membershipProtocols.push_back(std::make_unique<membership_protocol::MembershipProtocol>(addr, logger, membership_protocol::Config(1), messageDispatcher, failureDetectorFactory, goossipProtocolFactory));
        }

        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it)
        {
            (*it)->start();
        }

        // TODO: replace sleep with wait
        std::this_thread::sleep_for(timeout);

        int i = 0;
        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it, ++i)
        {
            auto members = (*it)->getMembers();
            auto membersNum = (*it)->getMembersNum();
            ASSERT_EQ(members.size(), membersNum);

            if (n == 2 || i == n - 1)
            {
                ASSERT_TRUE(members.empty());
                ASSERT_EQ(membersNum, 0);
            }
            else
            {
                ASSERT_FALSE(members.empty());
                ASSERT_EQ(membersNum, n - 2);
            }
        }


        for (auto it = membershipProtocols.rbegin(); it != membershipProtocols.rend(); ++it)
        {
            (*it)->stop();
        }
    }

    TEST(MembershipProtocolTests, NoAck)
    {
        test_n_nodes_one_failed(2, 5s);
    }

    TEST(MembershipProtocolTests, ThreeNodesOneFailed)
    {
        test_n_nodes_one_failed(3, 10s);
    }

    template <class Rep, class Period>
    void test_broken_link(int n, const std::chrono::duration<Rep, Period>& timeout)
    {
        auto logger = std::make_shared<utils::Log>();
        std::unique_ptr<failure_detector::IFailureDetectorFactory> failureDetectorFactory = std::make_unique<FailureDetectorFactory>();
        std::unique_ptr<gossip_protocol::IGossipProtocolFactory> goossipProtocolFactory = std::make_unique<GossipProtocolFactory>();
        
        std::vector<std::unique_ptr<membership_protocol::MembershipProtocol>> membershipProtocols;
        for (unsigned char i = 0; i < n; ++i)
        {
            network::Address addr(std::array<unsigned char, 4>{{static_cast<unsigned char>(i + 1), 0, 0, 0}}, (i + 1) * 100);
            std::shared_ptr<utils::MessageDispatcher> messageDispatcher;
            if (i < n - 1)
            {
                messageDispatcher = std::make_shared<utils::MessageDispatcher>(addr, logger);
            }
            else
            {
                network::Address otherAddress(std::array<unsigned char, 4>{{static_cast<unsigned char>(i), 0, 0, 0}}, (i) * 100);
                messageDispatcher = std::make_shared<BrokenLinkMessageDispatcher>(addr, otherAddress, logger);
            }
            membershipProtocols.push_back(std::make_unique<membership_protocol::MembershipProtocol>(addr, logger, membership_protocol::Config(1), messageDispatcher, failureDetectorFactory, goossipProtocolFactory));
        }

        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it)
        {
            (*it)->start();
        }

        // TODO: replace sleep with wait
        std::this_thread::sleep_for(timeout);

        int i = 0;
        for (auto it = membershipProtocols.begin(); it != membershipProtocols.end(); ++it, ++i)
        {
            auto members = (*it)->getMembers();
            ASSERT_FALSE(members.empty());
            auto membersNum = (*it)->getMembersNum();
            ASSERT_EQ(members.size(), membersNum);
            ASSERT_EQ(membersNum, n - 1);
        }

        for (auto it = membershipProtocols.rbegin(); it != membershipProtocols.rend(); ++it)
        {
            (*it)->requestStop();
        }

        for (auto it = membershipProtocols.rbegin(); it != membershipProtocols.rend(); ++it)
        {
            (*it)->stop();
        }
    }

    TEST(MembershipProtocolTests, BrokenLink_3nodes) 
    {
        test_broken_link(3, 10s);
    }

    TEST(MembershipProtocolTests, BrokenLink_5nodes) 
    {
        test_broken_link(5, 20s);
    }
}
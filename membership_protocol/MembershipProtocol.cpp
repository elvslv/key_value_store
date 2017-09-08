#include <sstream>

#include "MembershipProtocol.h"
#include "MembershipUpdate.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::unique_ptr<IFailureDetectorFactory>& failureDetectorFactory, const std::unique_ptr<IGossipProtocolFactory>& gossipProtocolFactory): 
        node(addr),
        network(std::make_shared<network::Network>(addr)),
        logger(logger),
        failureDetector(failureDetectorFactory->createFailureDetector(addr, logger, network, this)),
        gossipProtocol(gossipProtocolFactory->createGossipProtocol(addr, logger, network, this)),
        observers(),
        members()
    {
    }

    void MembershipProtocol::start()
    {
        // TODO: send initial ping

        failureDetector->addObserver(this);
        gossipProtocol->addObserver(this);

        failureDetector->start();
        gossipProtocol->start();
    }

    void MembershipProtocol::stop()
    {
        gossipProtocol->stop();
        failureDetector->stop();
    }

    void MembershipProtocol::addObserver(IMembershipProtocol::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void MembershipProtocol::onFailureDetectorEvent(const failure_detector::FailureDetectorEvent& failureDetectorEvent)
    {
        MembershipUpdateType membershipUpdateType;
        switch (failureDetectorEvent.eventType)
        {
            case failure_detector::ALIVE:
            {
                membershipUpdateType = JOINED;
                break;
            }

            case failure_detector::FAILED:
            {
                membershipUpdateType = FAILED;
                break;
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported event " << failureDetectorEvent.eventType;
                throw std::logic_error(ss.str());
            }
        }

        onMembershipUpdate(membershipUpdateType, FAILURE_DETECTOR, failureDetectorEvent.address);
    }

    void MembershipProtocol::onGossipEvent(const gossip_protocol::GossipEvent& gossipEvent)
    {
        MembershipUpdateType membershipUpdateType;
        switch (gossipEvent.eventType)
        {
            case gossip_protocol::JOINED:
            {
                membershipUpdateType = JOINED;
                break;
            }

            case gossip_protocol::FAILED:
            {
                membershipUpdateType = FAILED;
                break;
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported event " << gossipEvent.eventType;
                throw std::logic_error(ss.str());
            }
        }

        onMembershipUpdate(membershipUpdateType, GOSSIP_PROTOCOL, gossipEvent.address);
    }

    void MembershipProtocol::onMembershipUpdate(MembershipUpdateType membershipUpdateType, MembershipUpdateSource membershipUpdateSource, const network::Address& sourceAddress)
    {
        if (sourceAddress == node)
        {
            log("Received membership update ", membershipUpdateType, " from ",  membershipUpdateSource, " for current node itself. Ignoring it");
            return;
        }

        auto addressStr = sourceAddress.toString();
        log("Received membership update ", membershipUpdateType, " from ",  membershipUpdateSource, ", node address: ", addressStr);

        switch (membershipUpdateType)
        {
            case membership_protocol::JOINED:
            {
                bool newNode = members.find(addressStr) == members.end();
                if (newNode)
                {
                    members[addressStr] = Member(sourceAddress);

                    auto membershipUpdate = MembershipUpdate(members[addressStr], membership_protocol::JOINED);
                    onMembershipUpdate(membershipUpdate, membershipUpdateSource);
                }
                
                break;
            }

            case membership_protocol::FAILED:
            {
                bool nodeExists = members.find(addressStr) != members.end();
                if (nodeExists)
                {
                    members.erase(addressStr);

                    auto membershipUpdate = MembershipUpdate(members[addressStr], membership_protocol::FAILED);
                    onMembershipUpdate(membershipUpdate, membershipUpdateSource);
                }
            }

            default:
            {
                std::stringstream ss;
                ss << "unsupported membership update " << membershipUpdateType;
                throw std::logic_error(ss.str());
            }
        }
    }
    
    void MembershipProtocol::onMembershipUpdate(const MembershipUpdate& membershipUpdate, MembershipUpdateSource membershipUpdateSource)
    {
        for (auto observer : observers)
        {
            observer->onMembershipUpdate(membershipUpdate);
        }

        if (membershipUpdateSource == FAILURE_DETECTOR)
        {
            gossipProtocol->spreadMembershipUpdate(membershipUpdate);                    
        }

        log("Successfully notified observers about membership update");
    }
}
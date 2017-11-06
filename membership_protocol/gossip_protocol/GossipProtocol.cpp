#include "GossipProtocol.h"
#include "membership_protocol/messages/GossipMessage.h"
#include "membership_protocol/MembershipUpdate.h"
#include "utils/Utils.h"
#include "utils/Exceptions.h"

namespace gossip_protocol
{
    GossipProtocol::GossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol):
        address(addr),
        logger(logger),
        membershipProtocol(membershipProtocol),
        observers(),
        messageProcessingThread(),
        isRunning(false),
        period(0),
        gossips(),
        // periods(),
        gossipsMutex()
    {
    }

    void GossipProtocol::start()
    {
        isRunning = true;
        messageProcessingThread = std::make_unique<std::thread>(&GossipProtocol::run, this);
    }

    void GossipProtocol::stop()
    {
        isRunning = false;
        messageProcessingThread->join();
    }

    void GossipProtocol::run()
    {
        while (isRunning)
        {
            cleanupMessages();
            ++period;

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(100ms);
        }
    }

    void GossipProtocol::addObserver(IGossipProtocol::IObserver* observer)
    {
        observers.push_back(observer);
    }

    /*void GossipProtocol::onMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate)
    {
        switch (membershipUpdate.updateType)
        {
            case membership_protocol::JOINED:
            {
                members.insert(membershipUpdate.address);
                break;
            }
            
            case membership_protocol::FAILED:
            {
                members.remove(membershipUpdate.address);
                break;
            }
        }
    }*/

    void GossipProtocol::spreadMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate)
    {
        auto gossipId = utils::Utils::getRandomString(16);
        std::lock_guard<std::mutex> lock(gossipsMutex);
        gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {});
    }

    void GossipProtocol::onNewGossips(const network::Address& sourceAddress, const std::vector<membership_protocol::Gossip>& newGossips)
    {
        for (auto it = newGossips.begin(); it != newGossips.end(); ++it)
        {
            auto gossipId = it->id;
            std::lock_guard<std::mutex> lock(gossipsMutex);
            if (gossips.find(gossipId) != gossips.end())
            {
                logger->log("Already received gossip ", gossipId);
                continue;
            }

            auto membershipUpdate = membership_protocol::MembershipUpdate(it->address, it->membershipUpdateType);
            gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {sourceAddress});
            for (auto jt = observers.begin(); jt != observers.end(); ++jt)
            {
                (*jt)->onGossipEvent(membershipUpdate);
            }
        }
    }

    std::vector<membership_protocol::Gossip> GossipProtocol::getGossipsForAddress(const network::Address& address)
    {
        std::vector<membership_protocol::Gossip> result;
        int periodsToSpread = getPeriodsToSpread();
        int curPeriod = period;
        {
            std::lock_guard<std::mutex> lock(gossipsMutex);
            for (auto it = gossips.begin(); it != gossips.end(); ++it)
            {
                Gossip& gossip = it->second;
                if (gossip.period + periodsToSpread < curPeriod)
                {
                    continue;
                }

                if (gossip.infectedNodes.find(address) != gossip.infectedNodes.end())
                {
                    continue;
                }
    
                result.emplace_back(gossip.membershipUpdate.address, gossip.membershipUpdate.updateType, gossip.id);
                gossip.infectedNodes.insert(address);
            }    
        }

        return result;
    }

    void GossipProtocol::cleanupMessages()
    {
        int periodsToKeep = getPeriodsToKeep();
        int curPeriod = period;

        std::lock_guard<std::mutex> lock(gossipsMutex);
        for (auto it = gossips.begin(); it != gossips.end();)
        {
            Gossip& gossip = it->second;
            if (gossip.period + periodsToKeep >= curPeriod)
            {
                ++it;
                continue;
            }

            it = gossips.erase(it);
        }
    }

    int GossipProtocol::getPeriodsToSpread()
    {
        // TODO: figure out if this is the right formula
        return 2 * std::ceil(std::log2(membershipProtocol->getMembers().size() + 1));
    }

    int GossipProtocol::getPeriodsToKeep()
    {
        // TODO: figure out if this is the right formula
        int periodsToSpread = getPeriodsToSpread();
        return 2 * (periodsToSpread + 1);
    }
}
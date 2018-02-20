#include "GossipProtocol.h"
#include "membership_protocol/messages/GossipMessage.h"
#include "membership_protocol/MembershipUpdate.h"
#include "utils/Utils.h"
#include "utils/Exceptions.h"

namespace gossip_protocol
{
    GossipProtocol::GossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, membership_protocol::IMembershipProtocol* membershipProtocol, std::unique_ptr<utils::IThreadPolicy>& threadPolicy):
        address(addr),
        logger(logger),
        membershipProtocol(membershipProtocol),
        threadPolicy(std::move(threadPolicy)),
        observers(),
        period(0),
        gossips(),
        // periods(),
        gossipsMutex(),
        runnable([this](){run();}),
        noOpLock(),
        stopped(false)
    {
    }

    GossipProtocol::~GossipProtocol()
    {
        stop();
        logger->log(address, "[~GossipProtocol]");
    }

    void GossipProtocol::start()
    {
        logger->log(address, "[GossipProtocol::start]");

        runnable.start();
    }

    void GossipProtocol::stop()
    {
        if (stopped)
        {
            return;
        }

        runnable.stop();

        stopped = true;
        logger->log(address, "[GossipProtocol::stop]");
    }

    void GossipProtocol::run()
    {
        while (runnable.isRunning)
        {
            cleanupMessages();
            ++period;

            using namespace std::chrono_literals;
            runnable.condVar.wait_for(noOpLock, 1s, [this]{
                return !runnable.isRunning;
            });
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
        logger->log(address, "Adding gossip ", gossipId, ": ", membership_protocol::MembershipUpdate::UPDATE_TO_STR[membershipUpdate.updateType], "about ", membershipUpdate.address);
        std::lock_guard<std::mutex> lock(gossipsMutex);
        gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {membershipUpdate.address});
    }

    void GossipProtocol::onNewGossips(const network::Address& sourceAddress, const std::vector<membership_protocol::Gossip>& newGossips)
    {
        for (auto it = newGossips.begin(); it != newGossips.end(); ++it)
        {
            auto gossipId = it->id;
            std::lock_guard<std::mutex> lock(gossipsMutex);
            if (gossips.find(gossipId) != gossips.end())
            {
                logger->log(address, "Already received gossip ", gossipId);
                continue;
            }

            auto membershipUpdate = membership_protocol::MembershipUpdate(it->address, it->membershipUpdateType);
            logger->log(address, "Received gossip ", gossipId, " from ", it->address, " update ", membership_protocol::MembershipUpdate::UPDATE_TO_STR[it->membershipUpdateType]);
            gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {sourceAddress});
            for (auto jt = observers.begin(); jt != observers.end(); ++jt)
            {
                (*jt)->onGossipEvent(membershipUpdate);
            }
        }
    }

    std::vector<membership_protocol::Gossip> GossipProtocol::getGossipsForAddress(const network::Address& destAddress)
    {
        std::vector<membership_protocol::Gossip> result;
        int periodsToSpread = getPeriodsToSpread();
        int curPeriod = period;
        logger->log(address, "Looking for gossips for ", destAddress, " cur period: ", curPeriod, " periods to spread: ", periodsToSpread);
        {
            std::lock_guard<std::mutex> lock(gossipsMutex);
            for (auto it = gossips.begin(); it != gossips.end(); ++it)
            {
                Gossip& gossip = it->second;
                if (gossip.period + periodsToSpread <= curPeriod)
                {
                    continue;
                }

                if (gossip.infectedNodes.find(destAddress) != gossip.infectedNodes.end())
                {
                    continue;
                }
    
                result.emplace_back(gossip.membershipUpdate.address, gossip.membershipUpdate.updateType, gossip.id);
                gossip.infectedNodes.insert(destAddress);
            }
        }

        logger->log(address, "Found ", result.size(), " gossips for ", destAddress);
        for (int i = 0; i < result.size(); ++i)
        {
            logger->log(address, "Found gossip", result[i].id, "for ", destAddress);
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
        return 2 * std::ceil(std::log2(membershipProtocol->getMembersNum() + 1));
    }

    int GossipProtocol::getPeriodsToKeep()
    {
        // TODO: figure out if this is the right formula
        int periodsToSpread = getPeriodsToSpread();
        return 2 * (periodsToSpread + 1);
    }
}
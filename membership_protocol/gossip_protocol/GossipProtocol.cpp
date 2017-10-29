#include "GossipProtocol.h"
#include "membership_protocol/messages/GossipMessage.h"
#include "membership_protocol/MembershipUpdate.h"
#include "utils/Utils.h"
#include "utils/Exceptions.h"

namespace gossip_protocol
{
    GossipProtocol::GossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol):
        address(addr),
        logger(logger),
        messageDispatcher(messageDispatcher),
        tokens(),
        membershipProtocol(membershipProtocol),
        observers(),
        asyncQueue(std::bind(&GossipProtocol::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<membership_protocol::Message> message){asyncQueue.push(std::move(message));}),
        members(),
        messageProcessingThread(),
        isRunning(false),
        period(0),
        gossips(),
        periods(),
        gossipsMutex()
    {
    }

    void GossipProtocol::start()
    {
        membershipProtocol->addObserver(this);
        
        tokens[membership_protocol::GOSSIP] = messageDispatcher->listen(membership_protocol::GOSSIP, asyncQueueCallback);
        isRunning = true;
        messageProcessingThread = std::make_unique<std::thread>(&GossipProtocol::run, this);
    }

    void GossipProtocol::stop()
    {
        for (auto token : tokens)
        {
            messageDispatcher->stopListening(token.first, token.second);
        }

        isRunning = false;
        messageProcessingThread->join();
    }

    void GossipProtocol::run()
    {
        while (isRunning)
        {
            network::Address targetAddress;
            if (!members.getNextElement(targetAddress))
            {
                // TODO: add const
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                ++period;
                continue;
            }

            auto gossips = getGossipsForAddress(targetAddress);
            messageDispatcher->sendMessage(std::make_unique<membership_protocol::GossipMessage>(address, targetAddress, gossips), targetAddress);
        }
    }

    void GossipProtocol::addObserver(IGossipProtocol::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void GossipProtocol::onMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate)
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
    }

    void GossipProtocol::spreadMembershipUpdate(const membership_protocol::MembershipUpdate& membershipUpdate)
    {
        auto gossipId = utils::Utils::getRandomString(16);
        std::lock_guard<std::mutex> lock(gossipsMutex);
        gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {});
    }    

    void GossipProtocol::processMessage(const std::unique_ptr<membership_protocol::Message>& message)
    {
        switch (message->getMessageType())
        {
            case membership_protocol::GOSSIP:
            {
                {
                    auto gossipMessage = static_cast<membership_protocol::GossipMessage*>(message.get());
                    for (auto it = gossipMessage->getGossips().begin(); it != gossipMessage->getGossips().end(); ++it)
                    {
                        auto gossipId = it->id;
                        std::lock_guard<std::mutex> lock(gossipsMutex);
                        if (gossips.find(gossipId) != gossips.end())
                        {
                            logger->log("Already received gossip ", gossipId);
                            continue;
                        }

                        auto membershipUpdate = membership_protocol::MembershipUpdate(it->address, it->membershipUpdateType);
                        gossips[gossipId] = Gossip(period, gossipId, membershipUpdate, {message->getSourceAddress()});
                        for (auto jt = observers.begin(); jt != observers.end(); ++jt)
                        {
                            (*jt)->onGossipEvent(membershipUpdate);
                        }
                    }
                }

                break;
            }

            default:
                logger->log("Unexpected message ", message->toString());
                break;
        }
    }

    std::vector<membership_protocol::Gossip> GossipProtocol::getGossipsForAddress(const network::Address& address)
    {
        std::vector<membership_protocol::Gossip> result;
        {
            std::lock_guard<std::mutex> lock(gossipsMutex);
            
            for (auto it = gossips.begin(); it != gossips.end(); ++it)
            {
                Gossip& gossip = it->second;
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
        throw utils::NotImplementedException();
    }
}
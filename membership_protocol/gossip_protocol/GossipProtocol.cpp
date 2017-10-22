#include "GossipProtocol.h"

namespace membership_protocol
{
    GossipProtocol::GossipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, IMembershipProtocol* membershipProtocol):
        address(addr),
        logger(logger),
        messageDispatcher(messageDispatcher),
        tokens(),
        membershipProtocol(membershipProtocol),
        observers(),
        asyncQueue(std::bind(&GossipProtocol::processMessage, this, std::placeholders::_1)),
        asyncQueueCallback([this](std::unique_ptr<Message> message){asyncQueue.push(std::move(message));}),
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
        
        tokens[GOSSIP] = messageDispatcher->listen(GOSSIP, asyncQueueCallback);
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
            network::Address address;
            if (!members.getNextElement(address))
            {
                // TODO: add const
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(100ms);
                continue;
            }
        }
    }

    void GossipProtocol::addObserver(IGossipProtocol::IObserver* observer)
    {
        observers.push_back(observer);
    }

    void GossipProtocol::onMembershipUpdate(const MembershipUpdate& membershipUpdate)
    {
        auto address = membershipUpdate.member.address;
        switch (membershipUpdate.updateType)
        {
            case JOINED:
            {
                members.insert(address);
                break;
            }
            
            case FAILED:
            {
                members.remove(address);
                break;
            }
        }
    }

    void GossipProtocol::processMessage(const std::unique_ptr<Message>& message)
    {
        switch (message->getMessageType())
        {
            case GOSSIP:
            {
                auto gossipId = message->getId();
                {
                    std::lock_guard<std::mutex> lock(gossipsMutex);
                    auto it = gossips.find(gossipId);
                    if (it != gossips.end())
                    {
                        logger->log("Already received gossip ", gossipId);
                        return;
                    }

                    auto gossipMessage = std::static_cast<>;
                }

                break;
            }

            default:
                logger->log("Unexpected message ", message->toString());
                break;
        }
    }
}
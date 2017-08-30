#include <algorithm>
#include "MembershipProtocol.h"
#include "JoinRepMessage.h"
#include "PingMessage.h"
#include "AckMessage.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, const std::shared_ptr<utils::Log>& logger): 
        node(addr),
        members(),
        events(),
        waitingList(),
        waitingListIter(),
        network(addr),
        logger(logger)
    {
    }

    const std::vector<Node>& MembershipProtocol::getMembers() const
    {
        return members;
    }

    void MembershipProtocol::processMessage(const std::unique_ptr<Message> & message) 
    {
        log("Received message", message->getMessageType(), message->toString());
    
        auto sourceAddress = message->getSourceAddress();
        switch (message->getMessageType())
        {
            case JOINREQ:
            {
                addMemberListEntry(sourceAddress);
    
                auto repMessage = std::make_unique<JoinRepMessage>(node.getAddress(), sourceAddress, members);
                sendMessage(std::move(repMessage), sourceAddress);
                break;
            }
    
            case JOINREP:
            {
                // memberNode->inGroup = true;
                events.emplace_back(JOINED, node.getAddress(), utils::Time::getTimestamp());
                
                addMemberListEntry(sourceAddress);
    
                JoinRepMessage* repMessage = static_cast<JoinRepMessage*>(message.get());
                auto memberList = repMessage->getMemberList();
                for (auto it = memberList.begin(); it != memberList.end(); ++it)
                {
                    addMemberListEntry(it->getAddress());
                }
    
                break;
            }
    
            case PING:
            {
                PingMessage* pingMessage = static_cast<PingMessage*>(message.get());
                for (auto it = pingMessage->getEvents().begin(); it != pingMessage->getEvents().end(); ++it)
                {
                    log("received event ", it->getEventType(), " from node ", it->getNodeAddress().toString());
    
                    switch(it->getEventType())
                    {
                        case JOINED:
                        {
                            addMemberListEntry(it->getNodeAddress());
                            break;
                        }
    
                        case FAILED:
                        {
                            // throw std::logic_error("not implemented!");
                            log("node failed");
                        }
                    }
                }
    
                sendMessage(std::unique_ptr<AckMessage>(new AckMessage(node.getAddress(), sourceAddress)), sourceAddress);
                break;
            }
            case ACK:
            {
                std::string sourceAddressStr = sourceAddress.toString();
                if (waitingListIter.find(sourceAddressStr) == waitingListIter.end())
                {
                    throw std::logic_error("node not found");
                }
    
                log("Removing node ", sourceAddressStr, " from the waiting list");
                waitingList.erase(waitingListIter[sourceAddressStr]);
                waitingListIter.erase(sourceAddressStr);
    
                break;
            }
        }
    }

    void MembershipProtocol::sendMessage(const std::unique_ptr<Message>& message, const network::Address& destAddress)
    {
        auto msg = message->serialize();
        log("Sending message", msg.content.get());

        network.send(destAddress, msg);
    }

    void MembershipProtocol::addMemberListEntry(const network::Address& address) 
    {
        if (address == node.getAddress())
        {
            return;
        }

        auto it = std::find_if(members.begin(), members.end(), [&address] (const Node& node) { return node.getAddress() == address; });
        if (it != members.end())
        {
            return;
        }

        // logger->logNodeAdd(&memberNode->addr, &address);
        members.emplace_back(address);          
    }
}
#include <algorithm>
#include "MembershipProtocol.h"
#include "JoinRepMessage.h"
#include "PingMessage.h"
#include "AckMessage.h"
#include "../utils/TimeUtils.h"

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

    void MembershipProtocol::executeMembershipProtocol() 
    {
        for (auto it = waitingList.begin(); it != waitingList.end(); ++it)
        {
            // log("Ping message was sent to ", it->second.getAddress().getAddress(), " at ", it->first, " now ", par->globaltime);
    
            long curTime = utils::Time::getTimestamp();
            if (curTime - it->first > PING_TIMEOUT)
            {
                network::Address address = it->second.getAddress();
                std::string addressStr = address.toString();
                log("Node", addressStr, "exceeded timeout", PING_TIMEOUT);
                events.emplace_back(FAILED, address, curTime);
    
                assert(waitingListIter.erase(addressStr) == 1);
    
                // auto nodeIter = memberNode->memberListIter.find(addressStr);
                // assert(nodeIter != memberNode->memberListIter.end());
    
                // log(__FILE__, __LINE__);
    
                for (auto jt = members.begin(); jt != members.end(); ++jt)
                {
                    if (jt->getAddress() == address)
                    {
                        members.erase(jt);
                        break;
                    }
                }
    
                // log(__FILE__, __LINE__, addressStr);
    
                // memberNode->memberListIter.erase(addressStr);
    
                // log(__FILE__, __LINE__);
    
                it = std::prev(waitingList.erase(it));
    
                // logger->logNodeRemove(&memberNode->addr, &address);
            }
        }
    
        if (members.size() == 0)
        {
            return;
        }
    
        int i = std::rand() % members.size();
        auto targetAddress = members[i].getAddress();
        std::string addressStr = targetAddress.toString();
        if (waitingListIter.find(addressStr) != waitingListIter.end())
        {
            log("Already sent PING message to ", addressStr, " at ", waitingListIter[addressStr]->first);
            return;
        }
    
        long curTime = utils::Time::getTimestamp();        
        log("Failure detector is going to send PING message to node ", addressStr, " at ", curTime);
        
        waitingList.emplace_back(curTime, members[i]);
        waitingListIter[addressStr] = std::prev(waitingList.end());
        assert(waitingListIter[addressStr] !=  waitingList.end());
    
        std::vector<Event> newEvents;
        for (auto it = events.begin(); it != events.end(); ++it)
        {
            if (it->getTimestamp() > members[i].getTimestamp())
            {
                newEvents.push_back(*it);
            }
        }
    
        members[i].setTimestamp(curTime);
        sendMessage(std::make_unique<PingMessage>(node.getAddress(), targetAddress, newEvents), targetAddress);
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
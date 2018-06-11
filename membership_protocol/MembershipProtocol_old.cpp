#include <algorithm>
#include "MembershipProtocol.h"
#include "PingMessage.h"
#include "AckMessage.h"
#include "utils/TimeUtils.h"

namespace membership_protocol
{
    MembershipProtocol::MembershipProtocol(const network::Address& addr, std::shared_ptr<utils::Log> logger): 
        node(addr),
        network(addr),
        logger(logger),
        mutex(),
        members(),
        // membersSet(),
        membersMap(),
        events()
    {
    }

    std::vector<network::Address> MembershipProtocol::getMembers()
    {
        std::lock_guard<std::mutex> lock(mutex);

        std::vector<network::Address> result;
        result.reserve(members.size());

        for (auto it = members.begin(); it != members.end(); ++it)
        {
            result.push_back(*it);
        }

        return result;
    }

    void MembershipProtocol::processMessage(const std::unique_ptr<Message> & message) 
    {
        log("Received message", message->getMessageType(), message->toString());
    
        auto sourceAddress = message->getSourceAddress();
        switch (message->getMessageType())
        {
            case PING:
            {
                PingMessage* pingMessage = static_cast<PingMessage*>(message.get());
                std::vector<network::Address> newMembers;

                for (auto it = pingMessage->getEvents().begin(); it != pingMessage->getEvents().end(); ++it)
                {
                }
    
                addMembers(newMembers);

                sendMessage(std::unique_ptr<AckMessage>(new AckMessage(node.getAddress(), sourceAddress)), sourceAddress);
                break;
            }
            case ACK:
            {
                std::lock_guard<std::mutex> lock(mutex);
                
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

    void MembershipProtocol::processEvent(const Event& event)
    {
        log("received event ", event.getEventType(), " from node ", event.getNodeAddress().toString());
        
        switch(event.getEventType())
        {
            case JOINED:
            {
                auto address = event.getNodeAddress();
                auto it = membersMap.find(address);
                if (it == membersMap.end())
                {
                    addMember(address);
                }
                else
                {
                    int cnt = membersMap[address];
                    auto oldEv = events[cnt][address];
                    if (oldEv.getEventType() == JOINED)
                    {
                        log("received event ", event.getEventType(), " from node ", event.getNodeAddress().toString(), ", count: ", cnt, ". Ignoring it");
                        return;
                    }
                    else
                    {
                        assert(oldEv.getEventType() == FAILED);
                        events[cnt].erase(address);
                        events[0][address] = Event(JOINED, address);
                        membersMap[address] = 0;
                    }
                }
            }

            case FAILED:
            {
                // throw std::logic_error("not implemented!");
                log("node failed");
            }
        }
    }

    void MembershipProtocol::executeMembershipProtocol() 
    {
        std::lock_guard<std::mutex> lock(mutex);

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
    
                logNodeRemoved(address);
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
        log("Sending message to ", destAddress);

        network.send(destAddress, msg);
    }

    void MembershipProtocol::addMember(const network::Address& address) 
    {
        membersMap[address] = 0;
        events[0][address] = Event(JOINED, address);
    }

    /*void MembershipProtocol::addMembers(const std::vector<network::Address>& nodes) 
    {
        std::lock_guard<std::mutex> lock(mutex);
        
        for (auto jt = nodes.begin(); jt != nodes.end(); ++jt)
        {
            auto address = jt->getAddress();
            if (address == node.getAddress())
            {
                continue;
            }
    
            auto it = std::find_if(members.begin(), members.end(), [&address] (const Node& node) { return node.getAddress() == address; });
            if (it != members.end())
            {
                continue;
            }
    
            logNodeAdded(address);
            members.emplace_back(address);              
        }
    }*/

    void MembershipProtocol::logNodeAdded(const network::Address& nodeAddr)
    {
        log("Node", nodeAddr.toString(), "was added");
    }

    void MembershipProtocol::logNodeRemoved(const network::Address& nodeAddr)
    {
        log("Node", nodeAddr.toString(), "was removed");
    }

}
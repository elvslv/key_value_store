#pragma once

#include "IFailureDetector.h"
#include "membership_protocol/IMembershipProtocol.h"
#include "network/Network.h"
#include "utils/Log.h"
#include "utils/MessageDispatcher.h"

namespace membership_protocol
{
    class IFailureDetectorFactory
    {
    public:
        virtual ~IFailureDetectorFactory() {};
        virtual std::unique_ptr<IFailureDetector> createFailureDetector(const network::Address& addr, const std::shared_ptr<utils::Log>& logger, const std::shared_ptr<utils::MessageDispatcher>& messageDispatcher, membership_protocol::IMembershipProtocol* membershipProtocol) = 0;
    };
}
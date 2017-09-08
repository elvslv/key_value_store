#pragma once

#include "../network/Address.h"

namespace failure_detector
{
    enum FailureDetectorEventType
    {
        ALIVE,
        FAILED,
    };

    struct FailureDetectorEvent
    {
        network::Address address;
        FailureDetectorEventType eventType;
    };
}
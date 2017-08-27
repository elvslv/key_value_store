#include "ProtobufFactory.h"

namespace protobuf
{
    gen::EventTypes ProtobufFactory::getProtobufEventsType(membership_protocol::EventTypes eventType)
    {
        switch (eventType)
        {
            case membership_protocol::JOINED:
                return gen::JOINED;

            case membership_protocol::FAILED:
                return gen::FAILED;
        }
    }
}
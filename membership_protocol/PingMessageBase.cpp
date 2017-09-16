#include <sstream>
#include "PingMessageBase.h"

namespace membership_protocol
{
    PingMessageBase::PingMessageBase(MsgTypes msgType, const network::Address& from, const network::Address& to): 
        Message(msgType, from, to)
    {
    }
}
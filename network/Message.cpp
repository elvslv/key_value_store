#include "Message.h"

namespace network
{
    Message::Message():
        content(nullptr),
        size(0)
    {
    }

    Message::Message(char* data, unsigned int sz):
        content(data),
        size(sz)
    {
    }
}
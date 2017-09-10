#pragma once

#include <memory>

namespace network
{
    struct Message
    {
        Message();
        Message(char* data, unsigned int sz);
        bool empty();
        std::shared_ptr<char> content;
        unsigned int size;
    };
}
#pragma once

#include <memory>

namespace network
{
    struct Message
    {
        Message();
        std::shared_ptr<char> content;
        unsigned int size;
    };
}
#pragma once

#include <memory>
#include <string>

namespace network
{
    struct Message
    {
        Message();
        Message(char* data, unsigned int sz);
        Message(const std::string& data);
        
        friend bool operator==(const Message&, const Message&);
        bool empty();
        std::shared_ptr<char> content;
        unsigned int size;
    };
}
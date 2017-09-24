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

    Message::Message(const std::string& data):
        content(new char[data.size()]),
        size(data.size())
    {
        std::strcpy(content.get(), data.c_str());
    }


    bool operator==(const Message& m1, const Message& m2)
    {
        return m1.size == m2.size && std::strncmp(m1.content.get(), m2.content.get(), m1.size) == 0;
    }    

    bool Message::empty()
    {
        return size == 0;
    }
}
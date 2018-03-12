#pragma once

#include <string>

namespace key_value_store
{
    class INode
    {
    public:
        virtual ~INode(){}

        virtual void create(const std::string& key, const std::string& value) = 0;
        virtual std::string read(const std::string& key) = 0;
        virtual void update(const std::string& key, const std::string& value) = 0;
        virtual void remove(const std::string& key) = 0;
    };
}
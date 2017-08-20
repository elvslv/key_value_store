#include <sstream>
#include "Address.h"

namespace network
{
    std::string Address::toString() const
    {
        std::stringstream ss;
        for (auto it = address.begin(); it != address.end(); ++it)
        {
            if (it != address.begin())
            {
                ss << ".";
            }

            ss << *it;
        }

        ss << ":" << port;

        return ss.str();
    }
}
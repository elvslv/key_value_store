#include <sstream>
#include "Address.h"

namespace network
{
    /*Address::Address(const gen::Address& addr)
    {
        assert(addr.addr_size() == 4);
        for (int i = 0; i < 4; ++i)
        {
            address[i] = addr.addr(i);
        }

        port = addr.port();
    }

    std::unique_ptr<gen::Address> Address::serialize() const
    {
        gen::Address* addr = new gen::Address;
        for (auto it = address.begin(); it != address.end(); ++it)
        {
            addr->add_addr(*it);
        }

        addr->set_port(port);

        return std::unique_ptr<gen::Address>(addr);
    }*/

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
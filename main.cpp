#include "utils/Log.h"
#include "network/Address.h"

int main()
{
    utils::Log logger;
    logger.log("test", "123", "5.0");

    network::Address address({{1, 2, 3, 4}}, 5);

    return 0;
}
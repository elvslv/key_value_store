#include "Exceptions.h"

namespace key_value_store
{
NotFoundException::NotFoundException(const std::string& key)
    : key(key)
{
}

ConflictDetected::ConflictDetected(const std::string& key)
    : key(key)
{
}
}
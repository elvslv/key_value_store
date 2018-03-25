#include "RequestMessage.h"

namespace key_value_store
{
class UpdateRequestMessage : public RequestMessage
{
public:
    UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value, const std::string& id);
    UpdateRequestMessage(const network::Address& sourceAddress, const network::Address& destinationAddress, const std::string& key, const std::string& value);

protected:
    virtual gen::RequestMessage serializeToProtobuf() const;

private:
    std::string value;
};
}
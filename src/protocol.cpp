#include "protocol.hpp"

#include <cereal/archives/json.hpp>

namespace hyper_block {
namespace protocol {

std::string
authentication::serialize() const
{
    std::ostringstream oss;
    {
        cereal::JSONOutputArchive archive(oss);
        archive(*this);
    }
    return oss.str();
}

}   // namespace protocol
}   // namespace hyper_block

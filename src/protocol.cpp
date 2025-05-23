#include "protocol.hpp"

#include <cereal/archives/json.hpp>


namespace gameserver {
class authentication
{
    std::string serialize() const
    {
        std::ostringstream oss;
        {
            cereal::JSONOutputArchive archive(oss);
            archive(*this);
        }
        return oss.str();
    }
}
}   // namespace gameserver
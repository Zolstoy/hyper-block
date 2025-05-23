#pragma once

// #include <cereal/archives/json.hpp>
#include <sstream>
#include <string>

namespace gameserver {
namespace protocol {

struct authentication {
    std::string nickname;
    std::string password;

    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(nickname,
                password);   // serialize things by passing them to the archive
    }

    std::string serialize() const;
};

}   // namespace protocol
}   // namespace gameserver

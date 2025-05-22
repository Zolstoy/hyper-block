#pragma once

#include <cereal/archives/json.hpp>
#include <string>

namespace hyper_block {
namespace protocol {

struct authentication {
    std::string nickname;
    std::string password;

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(nickname, password);   // serialize things by passing them to the archive
    }
};

}   // namespace protocol
}   // namespace hyper_block

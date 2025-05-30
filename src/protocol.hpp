#pragma once

#include <cereal/archives/json.hpp>
#include <string>

namespace hyper_block {
namespace protocol {

template <typename T>
std::string
serialize(const T &obj)
{
    std::ostringstream oss;
    {
        cereal::JSONOutputArchive archive(oss);
        archive(obj);
    }
    return oss.str();
}

template <typename T>
T
deserialize(const std::string &str)
{
    std::istringstream iss(str);
    T                  obj;
    {
        cereal::JSONInputArchive archive(iss);
        archive(obj);
    }
    return obj;
}

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
}   // namespace hyper_block

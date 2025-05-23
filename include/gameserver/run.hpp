#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <stdexcept>
#include <string>

#include "boost/asio/io_context.hpp"

namespace gameserver {

template <typename T>
concept UserRoutines = requires(T t) {
    {
        t.authenticate(std::declval<std::string>())
    } -> std::same_as<bool>;
    {
        t.cycle(std::declval<double>)
    } -> std::same_as<void>;
};

template <UserRoutines T>
class instance
{
   private:
   public:
    static instance run(boost::asio::io_context &io_context, boost::asio::ssl::context &ssl_context, short port)
    {}
}

}   // namespace gameserver

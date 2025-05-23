#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
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
    boost::asio::io_context &io_context;
    boost::asio::ssl::context &ssl_context;
    boost::asio::ip::tcp::acceptor acceptor;

   private:
    instance(
        boost::asio::io_context &io_context, boost::asio::ssl::context &&ssl_context,
        boost::asio::ip::tcp::acceptor &&acceptor)
        : io_context(io_context)
        , ssl_context(ssl_context)
        , acceptor(std::forward<boost::asio::ip::tcp::acceptor>(acceptor))
    {}

   public:
    static instance run(
        boost::asio::io_context &io_context, boost::asio::ssl::context &ssl_context, unsigned short port)
    {
        auto inst = instance(
            io_context, std::move(ssl_context),
            boost::asio::ip::tcp::acceptor(io_context, {boost::asio::ip::tcp::v4(), port}));
    }
};

}   // namespace gameserver

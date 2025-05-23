#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <memory>
#include <string>

#include "session.hpp"

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

template <UserRoutines UR>
class instance : public std::enable_shared_from_this<instance<UR>>
{
   private:
    boost::asio::io_context       &io_context_;
    boost::asio::ssl::context     &ssl_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    UR                             user_routines_;
    std::vector<session<true>>     player_sessions_;
    std::vector<session<false>>    bot_sessions_;

   private:
    instance(boost::asio::io_context &io_context, boost::asio::ssl::context &&ssl_context, boost::asio::ip::tcp::acceptor &&acceptor)
        : io_context_(io_context)
        , ssl_context_(ssl_context)
        , acceptor_(std::forward<boost::asio::ip::tcp::acceptor>(acceptor))
    {}

   public:
    static std::shared_ptr<instance<UR>> run(boost::asio::io_context &io_context, boost::asio::ssl::context &ssl_context, unsigned short port)
    {
        auto inst = instance(io_context, std::move(ssl_context), boost::asio::ip::tcp::acceptor(io_context, {boost::asio::ip::tcp::v4(), port}));

        inst.acceptor.listen();
        inst.do_accept();
    }

    void do_accept()
    {
        acceptor_.async_accept(std::bind(&instance<UR>::on_accept, this->shared_from_this(), std::placeholders::_1, std::placeholders::_2));
    }

    void on_accept(const boost::system::error_code &ec, boost::asio::ip::tcp::socket socket)
    {
        if (!ec)
        {
            auto new_session = gameserver::session<true>::run(std::move(socket), ssl_context_);
        } else
        {
            // Handle error
        }
        do_accept();
    }
};

}   // namespace gameserver

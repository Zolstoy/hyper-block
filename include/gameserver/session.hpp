#pragma once

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cereal/archives/json.hpp>
#include <cstdlib>

#include "player.hpp"
#include "protocol.hpp"

namespace gameserver {

template <bool S = true>
class session : public std::enable_shared_from_this<session<S>>
{
   private:
    using protocol = boost::asio::ip::tcp;
    using plain_stream = boost::asio::basic_stream_socket<protocol>;
    using tls_stream = boost::asio::ssl::stream<boost::beast::tcp_stream>;
    using plain_websocket = boost::beast::websocket::stream<plain_stream>;
    using tls_websocket = boost::beast::websocket::stream<tls_stream>;
    using websocket = std::conditional_t<S, tls_websocket, plain_websocket>;

   private:
    websocket                 ws_;
    boost::beast::flat_buffer buffer_;
    player                    player_;

   public:
    static std::shared_ptr<session<false>> run(boost::asio::ip::tcp::socket &&socket);
    static std::shared_ptr<session<true>>  run_with_tls(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx);

    session(boost::asio::ip::tcp::socket &&socket)
        : ws_(std::move(socket))
    {}

    session(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx)
        : ws_(std::move(socket), ctx)
    {}

    void on_handshake(boost::beast::error_code ec)
    {
        if (ec)
        {
            std::cerr << "Handshake failed: " << ec.message() << std::endl;
            return;
        }
    }

    void on_accept(boost::beast::error_code ec)
    {
        if (ec)
        {
            std::cerr << "Accept failed: " << ec.message() << std::endl;
            return;
        }

        do_read();
    }

    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);

        if (ec == boost::beast::websocket::error::closed)
            return;

        if (ec)
        {
            std::cerr << "Read failed: " << ec.message() << std::endl;
            return;
        }

        std::string message = boost::beast::buffers_to_string(buffer_.data());
        buffer_.consume(buffer_.size());
        std::cout << "Received message: " << message << std::endl;

        std::stringstream                    ss(message);
        cereal::JSONInputArchive             iarchive(ss);

        gameserver::protocol::authentication auth;

        iarchive(auth);

        std::cout << "Nickname: " << auth.nickname << std::endl;
        std::cout << "Password: " << auth.password << std::endl;
    };
};

}   // namespace gameserver

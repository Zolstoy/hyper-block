#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <variant>

#include "player.hpp"

namespace gameserver {

class session : public std::enable_shared_from_this<session>
{
   private:
    using socket_type = boost::asio::ip::tcp::socket;
    using ssl_socket_type = boost::asio::ssl::stream<socket_type>;
    using websocket_type = boost::beast::websocket::stream<socket_type>;
    using ssl_websocket_type = boost::beast::websocket::stream<ssl_socket_type>;

   private:
    std::variant<websocket_type, ssl_websocket_type> ws_;
    boost::beast::flat_buffer                        buffer_;
    player                                           player_;

   public:
    session(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket);

    static std::shared_ptr<session> run(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket);
    static std::shared_ptr<session> run_with_tls(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket);

    void                            on_handshake(boost::beast::error_code ec);
    void                            on_accept(boost::beast::error_code ec);
    void                            do_read();
    void                            on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
};

}   // namespace gameserver

#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <variant>

#include "boost/asio/basic_stream_socket.hpp"
#include "boost/beast/core/tcp_stream.hpp"
#include "player.hpp"

namespace gameserver {

class session : public std::enable_shared_from_this<session>
{
   private:
    using socket = boost::asio::ip::tcp::socket;
    using plain_stream = boost::asio::basic_stream_socket<boost::asio::ip::tcp>;
    using tls_stream = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;
    using plain_websocket = boost::beast::websocket::stream<plain_stream>;
    using tls_websocket = boost::beast::websocket::stream<tls_stream>;

   private:
    std::variant<plain_websocket, tls_websocket> ws_;
    boost::beast::flat_buffer                    buffer_;
    player                                       player_;

   public:
    session(boost::asio::ip::tcp::socket &&socket);
    session(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx);

    static std::shared_ptr<session> run(boost::asio::ip::tcp::socket &&socket);
    static std::shared_ptr<session> run_with_tls(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx);

    void                            on_handshake(boost::beast::error_code ec);
    void                            on_accept(boost::beast::error_code ec);
    void                            do_read();
    void                            on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
};

}   // namespace gameserver

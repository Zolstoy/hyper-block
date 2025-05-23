#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <type_traits>

#include "boost/beast/core/tcp_stream.hpp"
#include "player.hpp"

namespace hyper_block {

template <bool SSL = true>
class session
{
   private:
    boost::beast::websocket::stream<typename std::conditional_t<
        SSL, boost::asio::ssl::stream<boost::beast::tcp_stream>, boost::beast::tcp_stream>::type>
        ws_;
    boost::beast::flat_buffer buffer_;
    player player_;

   public:
    session(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket socket);

    void run();
    void on_run();
    void on_handshake(boost::beast::error_code ec);
    void on_accept(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
};

template <>
class session<true> : public std::enable_shared_from_this<session<true>>
{
};

}   // namespace hyper_block

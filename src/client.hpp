#pragma once

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <memory>

namespace hyper_block {

class client : public std::enable_shared_from_this<client>
{
   private:
    boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> ws_;
    boost::beast::flat_buffer buffer_;

   public:
    client(boost::asio::ssl::context& ctx, boost::asio::ip::tcp::socket socket);

    void run();
    void on_run();
    void on_handshake(boost::beast::error_code ec);
    void on_accept(boost::beast::error_code ec);
    void do_read();
    void on_read(boost::beast::error_code ec, std::size_t bytes_transferred);
};

}   // namespace hyper_block

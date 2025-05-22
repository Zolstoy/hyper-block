#pragma once

#include <boost/asio/ssl.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>

namespace hyper_block {
namespace client {
class client
{
   private:
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    boost::beast::flat_buffer buffer_;

   public:
    bool authenticate();
};
}   // namespace client
}   // namespace hyper_block
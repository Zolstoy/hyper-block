#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

namespace hyper_block {

namespace server {
void run(boost::asio::io_context& ioc, short port, boost::asio::ssl::context& ctx);
}

}   // namespace hyper_block

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>

namespace hyper_block {

void run(const boost::asio::io_context& ioc, short port, boost::asio::ssl::context& ctx);

}   // namespace hyper_block

#include "server.hpp"

#include <algorithm>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "boost/asio/io_context.hpp"

using namespace boost;

namespace hyper_block {

void run(const boost::asio::io_context& ioc, short port, boost::asio::ssl::context& ctx);
{
    // Create and launch a listening port
    boost::asio::ip::tcp::acceptor acceptor(ioc, {boost::asio::ip::tcp::v4(), port});
    acceptor.listen();

    std::cout << "Server is running on port " << port << std::endl;

    acceptor.async_accept()
}

}   // namespace hyper_block

#include "server.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <iostream>

#include "boost/asio/io_context.hpp"

using namespace boost;

namespace hyper_block {

void
run(boost::asio::io_context& io_context, short port, boost::asio::ssl::context& ctx)
{
    asio::ip::tcp::acceptor acceptor(io_context, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), 13));

    acceptor.listen();

    std::cout << "Server is running on port " << port << std::endl;

    for (;;)
    {
        asio::ip::tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::cout << "New connection from " << socket.remote_endpoint() << std::endl;
        // Create a new thread for each connection
    }
}

}   // namespace hyper_block

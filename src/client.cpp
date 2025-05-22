#include "client.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cstdlib>
#include <iostream>

using namespace boost;

namespace hyper_block {

client::client(boost::asio::ssl::context& ctx, boost::asio::ip::tcp::socket socket)
    : ws_(std::move(socket), ctx)
{}

void
client::run()
{
    asio::dispatch(ws_.get_executor(), boost::beast::bind_front_handler(&client::on_run, shared_from_this()));
}

void
client::on_run()
{
    ws_.next_layer().async_handshake(
        asio::ssl::stream_base::server, beast::bind_front_handler(&client::on_handshake, shared_from_this()));
}

void
client::on_handshake(boost::beast::error_code ec)
{
    if (ec)
    {
        std::cerr << "Handshake failed: " << ec.message() << std::endl;
        return;
    }

    ws_.set_option(beast::websocket::stream_base::timeout::suggested(beast::role_type::server));

    ws_.async_accept(beast::bind_front_handler(&client::on_accept, shared_from_this()));
}

void
client::on_accept(boost::beast::error_code ec)
{
    if (ec)
    {
        std::cerr << "Accept failed: " << ec.message() << std::endl;
        return;
    }

    // Read a message
    do_read();
}

void
client::do_read()
{
    // Read a message into our buffer
    ws_.async_read(buffer_, beast::bind_front_handler(&client::on_read, shared_from_this()));
}

void
client::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
{
    boost::ignore_unused(bytes_transferred);

    // This indicates that the session was closed
    if (ec == beast::websocket::error::closed)
        return;

    if (ec)
    {
        std::cerr << "Read failed: " << ec.message() << std::endl;
        return;
    }
}

}   // namespace hyper_block

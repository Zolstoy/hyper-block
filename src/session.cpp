#include "session.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cereal/archives/json.hpp>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <sstream>
#include <variant>

#include "protocol.hpp"

using namespace boost;

namespace gameserver {

std::shared_ptr<session>
session::run(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(ctx, socket);
    std::get<websocket_type>(new_session->ws_).async_accept(std::bind(&session::on_accept, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

std::shared_ptr<session>
session::run_with_tls(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(ctx, socket);
    std::get<ssl_websocket_type>(new_session->ws_)
        .next_layer()
        .async_handshake(boost::asio::ssl::stream_base::server, std::bind(&session::on_handshake, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

session::session(boost::asio::ssl::context &ctx, boost::asio::ip::tcp::socket &&socket)
{}

void
session::on_handshake(boost::beast::error_code ec)
{
    if (ec)
    {
        std::cerr << "Handshake failed: " << ec.message() << std::endl;
        return;
    }
}

void
session::on_accept(boost::beast::error_code ec)
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
session::do_read()
{
    if (std::holds_alternative<websocket_type>(ws_))
        std::get<websocket_type>(ws_).async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
    else
        std::get<ssl_websocket_type>(ws_).async_read(buffer_, beast::bind_front_handler(&session::on_read, shared_from_this()));
}

void
session::on_read(boost::beast::error_code ec, std::size_t bytes_transferred)
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

    // Process the message
    std::string message = beast::buffers_to_string(buffer_.data());
    buffer_.consume(buffer_.size());
    std::cout << "Received message: " << message << std::endl;

    std::stringstream        ss(message);
    cereal::JSONInputArchive iarchive(ss);

    protocol::authentication auth;

    iarchive(auth);

    std::cout << "Nickname: " << auth.nickname << std::endl;
    std::cout << "Password: " << auth.password << std::endl;
}

}   // namespace gameserver

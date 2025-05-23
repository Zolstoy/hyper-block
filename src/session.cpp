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
#include <utility>
#include <variant>

#include "protocol.hpp"

using namespace boost;

namespace gameserver {

std::shared_ptr<session>
session::run(boost::asio::ip::tcp::socket &&socket)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(std::move(socket));
    std::get<plain_websocket>(new_session->ws_).async_accept(std::bind(&session::on_accept, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

std::shared_ptr<session>
session::run_with_tls(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(std::move(socket), ctx);
    std::get<tls_websocket>(new_session->ws_)
        .next_layer()
        .async_handshake(boost::asio::ssl::stream_base::server, std::bind(&session::on_handshake, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

session::session(boost::asio::ip::tcp::socket &&socket)
    : ws_(std::in_place_index<0>, std::move(socket))
{}

session::session(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx)
    : ws_(std::in_place_index<1>, std::move(socket), ctx)
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

template <typename T>
concept WebSocket = std::is_same_v<T, session::plain_websocket> || std::is_same_v<T, session::tls_websocket> auto T::do_read()
{}

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

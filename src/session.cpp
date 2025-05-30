#include "session.hpp"

#include <spdlog/spdlog.h>

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cereal/archives/json.hpp>

#include "boost/beast/core/bind_handler.hpp"
#include "session_base.hpp"

using namespace boost;

namespace hyper_block {

plain_session::plain_session(asio::ip::tcp::socket &&socket, std::function<step::callback> &step_callback)
    : session_base<false, plain_session>(plain_websocket(std::move(socket)), step_callback)
{}

tls_session::tls_session(asio::ip::tcp::socket &&socket, asio::ssl::context &ctx,
                         std::function<step::callback> &step_callback)
    : session_base<true, tls_session>(tls_websocket(std::move(socket), ctx), step_callback)
{}

void
plain_session::run_async()
{
    spdlog::debug("plain: initiating websocket upgrade");
    ws_.async_accept(beast::bind_front_handler(&session_base::on_accept, shared_from_this()));
}

void
tls_session::run_async()
{
    spdlog::debug("TLS: initiating handshake");
    ws_.next_layer().async_handshake(asio::ssl::stream_base::server,
                                     beast::bind_front_handler(&tls_session::on_handshake, shared_from_this()));
}

void
tls_session::on_handshake(beast::error_code ec)
{
    spdlog::debug("TLS: handshake done");
    HYPERBLOCK_STEP(session::handshake{});
    if (ec)
    {
        spdlog::warn("TLS: handshake failed");
        return;
    }
    ws_.async_accept(beast::bind_front_handler(&session_base::on_accept, shared_from_this()));
}

}   // namespace hyper_block

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

template <>
std::shared_ptr<session<false>>
session<false>::run(boost::asio::ip::tcp::socket &&socket)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(std::move(socket));
    new_session->ws_.async_accept(std::bind(&session::on_accept, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

template <>
std::shared_ptr<session<true>>
session<true>::run_with_tls(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx)
{
    std::shared_ptr<session> new_session = std::make_shared<session>(std::move(socket), ctx);
    new_session->ws_.next_layer().async_handshake(
        boost::asio::ssl::stream_base::server, std::bind(&session::on_handshake, new_session->shared_from_this(), std::placeholders::_1));
    return new_session;
}

}   // namespace gameserver

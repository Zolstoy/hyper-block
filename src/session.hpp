#pragma once

#include <boost/asio/basic_stream_socket.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/core/tcp_stream.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <cereal/archives/json.hpp>

#include "boost/beast/core/error.hpp"
#include "session_base.hpp"

namespace hyper_block {

class plain_session : public session_base<false, plain_session>
{
   public:
    plain_session(boost::asio::ip::tcp::socket &&socket, std::function<step::callback> &step_callback);

   public:
    virtual void run_async() override;
};

class tls_session : public session_base<true, tls_session>
{
   public:
    tls_session(boost::asio::ip::tcp::socket &&socket, boost::asio::ssl::context &ctx,
                std::function<step::callback> &step_callback);

   public:
    virtual void run_async() override;

   private:
    void on_handshake(boost::beast::error_code ec);
};

}   // namespace hyper_block

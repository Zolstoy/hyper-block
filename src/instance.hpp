#pragma once

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <expected>

#include "error.hpp"
#include "session.hpp"
#include "step.hpp"

namespace hyper_block {

class instance : std::enable_shared_from_this<instance>
{
   public:
   private:
    boost::asio::io_context&                        io_context_;
    std::shared_ptr<boost::asio::ssl::context>      ssl_context_;
    std::shared_ptr<boost::asio::ip::tcp::acceptor> acceptor_;
    std::vector<std::shared_ptr<tls_session>>       player_sessions_;
    std::vector<std::shared_ptr<plain_session>>     bot_sessions_;
    short                                           port_;
    std::string                                     cert_pem_;
    std::string                                     key_pem_;
    bool                                            is_running_;
    std::function<step::callback>                   step_callback_;

   public:
    instance(boost::asio::io_context& io_context, short port, std::string const& cert_pem, std::string const& key_pem,
             std::function<step::callback>&& step_callback = std::function<step::callback>()) noexcept;

   public:
    std::expected<short, error> run_async() noexcept;

   private:
    void do_accept();
    void on_accept(const boost::system::error_code& ec, boost::asio::ip::tcp::socket socket);
};

}   // namespace hyper_block

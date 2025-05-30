#include "instance.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <expected>

#include "boost/system/detail/error_code.hpp"
#include "error.hpp"
#include "session.hpp"
#include "step.hpp"

using namespace boost;

namespace hyper_block {

instance::instance(asio::io_context& io_context, short port, std::string const& cert_pem, std::string const& key_pem,
                   std::function<step::callback>&& step_callback) noexcept
    : io_context_(io_context)
    , port_(port)
    , cert_pem_(cert_pem)
    , key_pem_(key_pem)
    , is_running_(false)
    , step_callback_(step_callback)
{}

std::expected<short, error>
instance::run_async() noexcept
{
    if (is_running_)
        return std::unexpected(error::instance_already_running);

    acceptor_ = std::make_shared<boost::asio::ip::tcp::acceptor>(io_context_);
    try
    {
        acceptor_->open(boost::asio::ip::tcp::v4());
        acceptor_->set_option(boost::asio::socket_base::reuse_address(true));
        acceptor_->bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port_));
        acceptor_->listen();

        ssl_context_ = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
        ssl_context_->set_options(boost::asio::ssl::context::default_workarounds);
    } catch (...)
    {
        return std::unexpected(error::acceptor_failed);
    }

    boost::system::error_code ec;
    if (ssl_context_->use_certificate_chain(asio::buffer(cert_pem_), ec))
        return std::unexpected(error::invalid_certificate);
    if (ssl_context_->use_private_key(asio::buffer(key_pem_), boost::asio::ssl::context::pem, ec))
        return std::unexpected(error::invalid_private_key);
    try
    {
        do_accept();
    } catch (...)
    {
        return std::unexpected(error::acceptor_failed);
    }
    is_running_ = true;
    return 0;
}

void
instance::do_accept()
{
    acceptor_->async_accept(std::bind(&instance::on_accept, this, std::placeholders::_1, std::placeholders::_2));
    spdlog::trace("server: listenning");
}

void
instance::on_accept(const boost::system::error_code& ec, boost::asio::ip::tcp::socket socket)
{
    HYPERBLOCK_STEP(server::accept{});
    if (ec)
    {
        spdlog::warn("server: accept failed");
        is_running_ = false;
        return;
    }
    spdlog::info("server: new connection");

    auto new_session = std::make_shared<tls_session>(std::move(socket), *ssl_context_, step_callback_);
    new_session->run_async();
    player_sessions_.push_back(new_session);

    do_accept();
}

}   // namespace hyper_block

#pragma once

#include <boost/asio/io_context.hpp>
#include <boost/asio/ssl.hpp>
#include <concepts>

namespace hyper_block {

bool all_pass(std::string) { return true; }

bool all_fail(std::string) { return false; }

namespace server {

template <class T>
concept AuthCallback = requires(T a) {
  { a } -> std::invocable<bool(std::string)>;
};

template <typename T>
void run(std::string cert_pem, std::string key_pem,
         T authentication_callback = all_pass) {
  run(0, cert_pem, key_pem, authentication_callback);
}

template <typename T>
void run(short port, std::string cert_pem, std::string key_pem,
         T authentication_callback = all_pass) {}
} // namespace server

} // namespace hyper_block

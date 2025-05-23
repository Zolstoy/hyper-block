#include <boost/asio/ssl.hpp>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "server.hpp"

using namespace boost;

int
main(int argc, char **argv)
{
    program_options::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
        "port,p", program_options::value<int>()->default_value(8080), "set port number")(
        "cert,c", program_options::value<std::string>(), "set certificate file")(
        "key,k", program_options::value<std::string>(), "set private key file");

    // program_options::positional_options_description p;

    program_options::variables_map vm;
    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);
    program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    auto port = vm["port"].as<short>();
    auto cert_path = vm["cert"].as<std::string>();
    auto key_path = vm["key"].as<std::string>();

    std::ifstream cert_file(cert_path);
    std::ostringstream os;
    os << cert_file.rdbuf();
    std::string cert = os.str();

    std::ifstream key_file(key_path);
    os.str("");
    os.clear();
    os << key_file.rdbuf();
    std::string key = os.str();

    asio::ssl::context ctx{asio::ssl::context::tlsv12};
    ctx.use_certificate_chain(boost::asio::buffer(cert.data(), cert.size()));
    ctx.use_private_key(boost::asio::buffer(key.data(), key.size()), boost::asio::ssl::context::file_format::pem);

    auto ioc = boost::asio::io_context{1};

    hyper_block::server::run(ioc, port, ctx);
}

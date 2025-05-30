#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>

#include "boost/asio/io_context.hpp"
#include "../instance.hpp"

using namespace boost;

#include <boost/numeric/ublas/vector.hpp>

class player
{
   private:
    std::string                           nickname_;
    boost::numeric::ublas::vector<double> position_;
    bool                                  facing_right_;
    bool                                  is_moving_;

   public:
    std::string get_nickname() const;
    void        set_nickname(const std::string& nickname);

    boost::numeric::ublas::vector<double> get_position() const;
    void                                  set_position(const boost::numeric::ublas::vector<double>& position);

    bool is_facing_right() const;
    void set_facing_right(bool facing_right);

    bool is_moving() const;
    void set_moving(bool is_moving);
};

int
main(int argc, char** argv)
{
    program_options::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("port,p", program_options::value<int>()->default_value(4567),
                                                         "set port number")(
        "cert,c", program_options::value<std::string>(), "set certificate file")(
        "key,k", program_options::value<std::string>(), "set private key file");

    program_options::variables_map vm;
    program_options::store(program_options::parse_command_line(argc, argv, desc), vm);
    program_options::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << "\n";
        return 1;
    }

    // auto port = vm["port"].as<short>();
    auto cert_path = vm["cert"].as<std::string>();
    auto key_path  = vm["key"].as<std::string>();

    std::ifstream      cert_file(cert_path);
    std::ostringstream os;
    os << cert_file.rdbuf();
    std::string cert = os.str();

    std::ifstream key_file(key_path);
    os.str("");
    os.clear();
    os << key_file.rdbuf();
    std::string key = os.str();

    asio::io_context ioc;
    auto             instance = std::make_shared<hyper_block::instance>(ioc, 4567, cert, key);
    if (!instance->run_async()) {
        std::cerr << "Failed to start instance" << std::endl;
        return 1;
    }
    for (int i = 0; i < std::thread::hardware_concurrency(); ++i)
    {
        ioc.run();
    }
}

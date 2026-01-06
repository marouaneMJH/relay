#include "relay/relay.hpp"
#include <iostream>

namespace relay
{

    RelayServer::RelayServer(unsigned short port)
        : port_(port)
    {
        std::cout << "RelayServer initialized on port " << port_ << std::endl;
    }

    void RelayServer::run()
    {
        std::cout << "RelayServer running..." << std::endl;
        io_context_.
    }

    void RelayServer::stop()
    {
        io_context_.stop();
        std::cout << "RelayServer stopped." << std::endl;
    }

    void server_logic()
    {
        tcp::socket socket(io_context_);
        boost::asio::connect(socket, "/hello");
        for (;;)
        {
            std::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
        }
    } // namespace relay

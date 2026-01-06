#include "relay/relay.hpp"
#include <iostream>

namespace relay
{

    // =====================
    // Session
    // =====================

    Session::Session(tcp::socket socket)
        : socket_(std::move(socket))
    {
    }

    void Session::start()
    {
        do_read();
    }

    void Session::do_read()
    {
        auto self = shared_from_this();

        socket_.async_read_some(
            boost::asio::buffer(buffer_),
            [this, self](boost::system::error_code ec, std::size_t length)
            {
                if (!ec)
                {
                    std::cout.write(buffer_.data(), length);
                    std::cout << std::endl;
                    do_read(); // continue reading
                }
            });
    }

    // =====================
    // RelayServer
    // =====================

    RelayServer::RelayServer(unsigned short port)
        : acceptor_(
              io_context_,
              tcp::endpoint(tcp::v4(), port))
    {
        std::cout << "RelayServer listening on port " << port << std::endl;
    }

    void RelayServer::run()
    {
        do_accept();
        io_context_.run();
    }

    void RelayServer::stop()
    {
        io_context_.stop();
        std::cout << "RelayServer stopped." << std::endl;
    }

    void RelayServer::do_accept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket))->start();
                }

                do_accept();
            });
    }
    // std::string RelayServer::make_daytime_string()
    // {
    //     std::time_t now = time(0);
    //     return ctime(now);
    // }
} // namespace relay

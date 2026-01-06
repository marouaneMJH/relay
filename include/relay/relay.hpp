#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace relay
{
    using boost::asio::ip::tcp;
    class Session : public std::enable_shared_from_this<Session>
    {
    public:
        explicit Session(tcp::socket socket);
        void start();

    private:
        void do_read();

        tcp::socket socket_;
        std::array<char, 1024> buffer_;
    };

    class RelayServer
    {
    public:
        explicit RelayServer(unsigned short port);
        void run();
        void stop();
        void do_accept();

        // std::string make_daytime_string();

        boost::asio::io_context io_context_;
        tcp::acceptor acceptor_;
    };

} // namespace relay

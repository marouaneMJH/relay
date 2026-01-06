#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#define MAX 10
#define BUFFER_SIZE 1024

using boost::asio::io_context;
using boost::asio::ip::tcp;

extern int counter = 0;

class RequestManager
{

public:
    // static std::size_t counter;

    static void
    manage_request(std::array<char, BUFFER_SIZE> data, std::size_t len)
    {
        std::cout << "#" << counter++ << ": ";
        std::cout.write(data.data(), len);
        std::cout << std::endl;
    }
};

class Session : public std::enable_shared_from_this<Session>
{
private:
    tcp::socket socket_;
    std::array<char, BUFFER_SIZE> data_;

    void do_read_()
    {
        auto self = shared_from_this();

        socket_.async_read_some(
            boost::asio::buffer(data_),
            [self, this](boost::system::error_code ec, std::size_t length) -> void
            {
                if (!ec)
                {
                    do_write_(length);
                }
            });
    }

    void do_write_(std::size_t len)
    {
        auto self = shared_from_this();
        boost::asio::async_write(
            socket_, boost::asio::buffer(data_, len),
            [this, self](boost::system::error_code ec, std::size_t len2)
            {
                if (!ec)
                {

                    RequestManager::manage_request(data_, len2);

                    do_read_();
                }
            });
    }

public:
    explicit Session(tcp::socket socket)
        : socket_(std::move(socket))
    {
        socket_.set_option(tcp::no_delay(true));
    }

    void start() { do_read_(); }
};

class Server
{
public:
    Server(boost::asio::io_context &io, short port)
        : acceptor_(io, tcp::endpoint(tcp::v4(), port))
    {
        std::cout << "Server running on port: " << port << "\n";
        do_accept_();
    }

private:
    tcp::acceptor acceptor_;

    void do_accept_()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket)
            {
                if (!ec)
                {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept_();
            });
    };
};

int main()
{
    // RequestManager::counter = 0;
    counter = 0;
    short port = 8080;
    try
    {
        boost::asio::io_context io;
        Server server(io, port);
        io.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

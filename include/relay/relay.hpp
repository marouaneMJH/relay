#ifndef RELAY_HPP
#define RELAY_HPP

#include <boost/asio.hpp>
#include <string>

namespace relay
{

    class RelayServer
    {
    public:
        RelayServer(unsigned short port);
        void run();
        void stop();

    private:
        boost::asio::io_context io_context_;
        unsigned short port_;
        void server_logic_();
    };

} // namespace relay

#endif // RELAY_HPP

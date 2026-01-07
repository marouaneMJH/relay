#pragma once
#include <boost/asio.hpp>
#include <iostream>

#include "peer_manager.hpp"
#include "router.hpp"

using boost::asio::ip::tcp;

class Node
{
public:
    Node(uint64_t id, uint16_t port);
    void run();
    void connect(const tcp::endpoint &ep);
    void send(uint64_t dst, std::string_view data);

private:
    void accept_loop();

    boost::asio::io_context io_;
    tcp::acceptor acceptor_;

    PeerManager peers_;
    Router router_;

    uint64_t id_;
};

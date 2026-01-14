#pragma once
#include <boost/asio.hpp>
#include <iostream>
#include <functional>

#include "peer_manager.hpp"
#include "router.hpp"

using boost::asio::ip::tcp;

class Node
{
public:
    using ReceiveHandler = std::function<void(uint64_t node_id, uint64_t from_id, const std::string &message)>;

    Node(uint64_t id, uint16_t port);
    Node(const Node &node);

    void run();
    void connect(const tcp::endpoint &ep);
    void send(uint64_t dst, std::string_view data);
    void set_receive_handler(ReceiveHandler handler);

    // Get the receive handler for router to use
    const ReceiveHandler &get_receive_handler() const { return receive_handler_; }

private:
    void accept_loop();
    static void default_receive_handler(uint64_t node_id, uint64_t from_id, const std::string &message);

    boost::asio::io_context io_;
    tcp::acceptor acceptor_;

    PeerManager peers_;
    Router router_;

    uint64_t id_;
    ReceiveHandler receive_handler_;
};

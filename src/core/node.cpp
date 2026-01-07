#include "core//node.hpp"
#include "core/peer_connection.hpp"

Node::Node(uint64_t id, uint16_t port)
    : acceptor_(io_, tcp::endpoint(tcp::v4(), port)),
      router_(id, peers_),
      id_(id)
{

    std::cout << this->id_ << "is initialised" << std::endl;
}

void Node::run()
{
    accept_loop();
    io_.run();
}

void Node::accept_loop()
{
    acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
                           {
        if (!ec) {
            auto peer = std::make_shared<PeerConnection>(std::move(socket), router_);
            peers_.add(peer);
            peer->start();
        }
        accept_loop(); });
}

void Node::connect(const tcp::endpoint &ep)
{
    auto socket = tcp::socket(io_);
    socket.async_connect(ep, [this, s = std::move(socket)](boost::system::error_code ec) mutable
                         {
        if (!ec) {
            auto peer = std::make_shared<PeerConnection>(std::move(s), router_);
            peers_.add(peer);
            peer->start();
            // DEBUG
            std::cout << this->id_  << "is connected";
        } });
}

void Node::send(uint64_t dst, std::string_view data)
{
    Message msg;
    msg.header.type = static_cast<uint16_t>(MessageType::Data);
    msg.header.src_node_id = id_;
    msg.header.dst_node_id = dst;
    msg.header.ttl = 8;
    msg.header.size = data.size();

    msg.payload.assign(data.begin(), data.end());

    peers_.for_each([&](auto &peer)
                    { peer->async_send(msg); });
}

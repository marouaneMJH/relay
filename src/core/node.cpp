#include "core//node.hpp"
#include "core/peer_connection.hpp"

Node::Node(uint64_t id, uint16_t port)
    : acceptor_(io_, tcp::endpoint(tcp::v4(), port)),
      router_(id, peers_, *this),
      id_(id),
      receive_handler_(default_receive_handler)
{

    std::cout << this->id_ << " is initialised" << std::endl;
}

void Node::set_receive_handler(ReceiveHandler handler)
{
    receive_handler_ = std::move(handler);
}

void Node::default_receive_handler(uint64_t node_id, uint64_t from_id, const std::string &message)
{
    std::cout << "[NODE " << node_id << "] received from " << from_id << ": " << message << std::endl;
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
            std::cout << "\n[NODE " << this->id_ << "] Accepted connection from " 
                      << socket.remote_endpoint() << std::endl;
            auto peer = std::make_shared<PeerConnection>(std::move(socket), router_);
            peers_.add(peer);
            peer->start();
        }
        accept_loop(); });
}

void Node::connect(const tcp::endpoint &ep)
{
    auto socket_ptr = std::make_shared<tcp::socket>(io_);
    socket_ptr->async_connect(ep, [this, socket_ptr, ep](boost::system::error_code ec) mutable
                              {
        if (!ec) {
            auto peer = std::make_shared<PeerConnection>(std::move(*socket_ptr), router_);
            peers_.add(peer);
            peer->start();
            std::cout << "\n[NODE " << this->id_ << "] Connected to " << ep << std::endl;
        } else {
            std::cerr << "\n[NODE " << this->id_ << "] Connection failed: " << ec.message() << std::endl;
        } });
}

void Node::send(uint64_t dst, std::string_view data)
{
    Message msg;
    msg.header.type = static_cast<uint16_t>(MessageType::Data);
    msg.header.src_node_id = id_;
    msg.header.dst_node_id = dst;
    msg.header.ttl = 2;
    msg.header.size = data.size();

    msg.payload.assign(data.begin(), data.end());

    std::cout << "\n[NODE " << id_ << "] Sending message to " << dst
              << " via peers" << std::endl;
    peers_.for_each([&](auto &peer)
                    { peer->async_send(msg); });
}

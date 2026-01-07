#include "core/router.hpp"
#include "core/peer_connection.hpp"
#include "core/peer_manager.hpp"
#include <iostream>

Router::Router(uint64_t self_id, PeerManager &peers)
    : self_id_(self_id), peers_(peers) {}

void Router::on_message(Message msg, PeerConnection *from)
{
    if (msg.header.dst_node_id == self_id_)
    {
        std::string text(msg.payload.begin(), msg.payload.end());
        std::cout << "[NODE " << self_id_ << "] received: " << text << std::endl;
        return;
    }

    if (msg.header.ttl == 0)
        return;

    msg.header.ttl--;
    forward(msg, from);
}

void Router::forward(Message &msg, PeerConnection *from)
{
    peers_.for_each([&](auto &peer)
                    {
        if (peer.get() != from) {
            peer->async_send(msg);
        } });
}

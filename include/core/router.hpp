#pragma once
#include <cstdint>

#include "message.hpp"

class PeerConnection;
class PeerManager;

class Router
{
public:
    Router(uint64_t self_id, PeerManager &peers);

    void on_message(Message msg, PeerConnection *from);

private:
    void forward(Message &msg, PeerConnection *from);

    uint64_t self_id_;
    PeerManager &peers_;
};

#pragma once
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>
#include "node.hpp"

// TODO: Load network from file

class CliManager
{
public:
    CliManager();
    ~CliManager();

    void run();
    void add_client(uint64_t id, uint16_t port);
    void connect_client(uint64_t client_id, const std::string &host, uint16_t port);
    void send_message(uint64_t from_id, uint64_t to_id, const std::string &message);
    void list_clients();
    void stop_client(uint64_t id);
    void stop_all();

private:
    struct ClientInfo
    {
        std::unique_ptr<Node> node;
        std::unique_ptr<std::thread> thread;
        boost::asio::io_context *io_context;
        uint64_t id;
        uint16_t port;
        bool running;
    };

    std::unordered_map<uint64_t, ClientInfo> clients_;
    std::mutex clients_mutex_;
    bool running_;

    void print_help();
    void process_command(const std::string &command);
};

#include "core/cli_manager.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>

CliManager::CliManager() : running_(false) {}

CliManager::~CliManager()
{
    stop_all();
}

void CliManager::run()
{
    running_ = true;
    std::cout << "CLI Manager started. Type 'help' for available commands.\n";
    print_help();

    std::string line;
    while (running_ && std::getline(std::cin, line))
    {
        if (line.empty())
            continue;

        try
        {
            process_command(line);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
}

void CliManager::add_client(uint64_t id, uint16_t port)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    if (clients_.find(id) != clients_.end())
    {
        std::cout << "Client with ID " << id << " already exists.\n";
        return;
    }

    try
    {
        ClientInfo info;
        info.id = id;
        info.port = port;
        info.running = true;
        info.node = std::make_unique<Node>(id, port);

        // Capture raw pointer for thread - the unique_ptr in the map keeps it alive
        Node *node_ptr = info.node.get();
        info.thread = std::make_unique<std::thread>([node_ptr]()
                                                    { node_ptr->run(); });

        clients_[id] = std::move(info);
        std::cout << "Client " << id << " started on port " << port << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to start client " << id << ": " << e.what() << "\n";
    }
}

void CliManager::connect_client(uint64_t client_id, const std::string &host, uint16_t port)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    auto it = clients_.find(client_id);
    if (it == clients_.end())
    {
        std::cout << "Client " << client_id << " not found.\n";
        return;
    }

    try
    {
        tcp::endpoint ep(boost::asio::ip::make_address(host), port);
        it->second.node->connect(ep);
        std::cout << "Client " << client_id << " connecting to " << host << ":" << port << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to connect client " << client_id << ": " << e.what() << "\n";
    }
}

void CliManager::send_message(uint64_t from_id, uint64_t to_id, const std::string &message)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    auto it = clients_.find(from_id);
    if (it == clients_.end())
    {
        std::cout << "Client " << from_id << " not found.\n";
        return;
    }

    try
    {
        it->second.node->send(to_id, message);
        std::cout << "Message sent from " << from_id << " to " << to_id << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Failed to send message: " << e.what() << "\n";
    }
}

void CliManager::list_clients()
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    if (clients_.empty())
    {
        std::cout << "No clients running.\n";
        return;
    }

    std::cout << "Active clients:\n";
    for (const auto &[id, info] : clients_)
    {
        std::cout << "  ID: " << id << ", Port: " << info.port
                  << ", Status: " << (info.running ? "Running" : "Stopped") << "\n";
    }
}

void CliManager::stop_client(uint64_t id)
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    auto it = clients_.find(id);
    if (it == clients_.end())
    {
        std::cout << "Client " << id << " not found.\n";
        return;
    }

    if (it->second.thread && it->second.thread->joinable())
    {
        // Note: This is a graceful approach - in production you'd want proper shutdown signaling
        it->second.running = false;
        it->second.thread->join();
    }

    clients_.erase(it);
    std::cout << "Client " << id << " stopped.\n";
}

void CliManager::stop_all()
{
    std::lock_guard<std::mutex> lock(clients_mutex_);

    for (auto &[id, info] : clients_)
    {
        if (info.thread && info.thread->joinable())
        {
            info.running = false;
            info.thread->join();
        }
    }

    clients_.clear();
    running_ = false;
    std::cout << "All clients stopped.\n";
}

void CliManager::print_help()
{
    std::cout << "\nAvailable commands:\n"
              << "  add <id> <port>              - Add and start a new client\n"
              << "  connect <client_id> <host> <port> - Connect client to another node\n"
              << "  send <from_id> <to_id> <message>  - Send message from one client to another\n"
              << "  list                         - List all active clients\n"
              << "  stop <id>                    - Stop a specific client\n"
              << "  stopall                      - Stop all clients\n"
              << "  help                         - Show this help message\n"
              << "  quit / exit                  - Exit the CLI manager\n\n";
}

void CliManager::process_command(const std::string &command)
{
    std::istringstream iss(command);
    std::string cmd;
    iss >> cmd;

    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "add")
    {
        uint64_t id;
        uint16_t port;
        if (!(iss >> id >> port))
        {
            std::cout << "Usage: add <id> <port>\n";
            return;
        }
        add_client(id, port);
    }
    else if (cmd == "connect")
    {
        uint64_t client_id;
        std::string host;
        uint16_t port;
        if (!(iss >> client_id >> host >> port))
        {
            std::cout << "Usage: connect <client_id> <host> <port>\n";
            return;
        }
        connect_client(client_id, host, port);
    }
    else if (cmd == "send")
    {
        uint64_t from_id, to_id;
        std::string message;
        if (!(iss >> from_id >> to_id))
        {
            std::cout << "Usage: send <from_id> <to_id> <message>\n";
            return;
        }
        std::getline(iss, message);
        // Trim leading whitespace
        message.erase(0, message.find_first_not_of(" \t"));
        if (message.empty())
        {
            std::cout << "Message cannot be empty.\n";
            return;
        }
        send_message(from_id, to_id, message);
    }
    else if (cmd == "list")
    {
        list_clients();
    }
    else if (cmd == "stop")
    {
        uint64_t id;
        if (!(iss >> id))
        {
            std::cout << "Usage: stop <id>\n";
            return;
        }
        stop_client(id);
    }
    else if (cmd == "stopall")
    {
        stop_all();
    }
    else if (cmd == "help")
    {
        print_help();
    }
    else if (cmd == "quit" || cmd == "exit")
    {
        stop_all();
        running_ = false;
    }
    else
    {
        std::cout << "Unknown command: " << cmd << ". Type 'help' for available commands.\n";
    }
}

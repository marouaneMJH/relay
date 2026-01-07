#include "core/node.hpp"
#include "core/cli_manager.hpp"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    // Check if running in CLI manager mode or single node mode
    if (argc == 1 || (argc == 2 && std::string(argv[1]) == "--cli"))
    {
        // CLI Manager mode - manage multiple clients interactively
        CliManager manager;
        manager.run();
        return 0;
    }

    // Single node mode (legacy behavior)
    if (argc < 3)
    {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " --cli               - Start CLI manager for multiple clients\n";
        std::cout << "  " << argv[0] << " <id> <port>        - Start single node\n";
        std::cout << "  " << argv[0] << " <id> <port> <peer_host> <peer_port> - Start node and connect to peer\n";
        return 1;
    }

    uint64_t id = std::stoull(argv[1]);
    uint16_t port = std::stoi(argv[2]);

    Node node(id, port);

    if (argc == 5)
    {
        node.connect({boost::asio::ip::make_address(argv[3]),
                      static_cast<uint16_t>(std::stoi(argv[4]))});
    }

    node.run();
    return 0;
}

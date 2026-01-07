#include "core/node.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cout << "usage: node <id> <port>\n";
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
}

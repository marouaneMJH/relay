#include "relay/relay.hpp"
#include <iostream>

int main()
{
    try
    {
        std::cout << "Starting Relay Application..." << std::endl;

        relay::RelayServer server(8080);
        server.run();

        while ()

            std::cout << "Relay Application finished successfully." << std::endl;
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}

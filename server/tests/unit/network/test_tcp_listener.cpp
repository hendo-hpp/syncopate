#include "config/network.hpp"
#include "network/tcp_listener.hpp"

#include <iostream>
#include <unistd.h>

int main() {
    std::cout << "[TEST]: initialize tcp listener.\n";
    syncopate::network::tcp_listener server(syncopate::config::network::DEFAULT_PORT);

    if (!server.start()) {
        std::cerr << "[TEST]: failed to start listener.\n";
        return -1;
    }

    std::cout << "[TEST]: server is listening, run 'curl http://localhost:8080' from another terminal\n";
    int client_fd = server.accept_connection();

    if (client_fd < 0) {
        std::cerr << "[TEST]: failed to connect to server.\n";
        return -1;
    }

    std::cout << "[TEST]: successfully connected on fd: " << client_fd << ".\n";
    close(client_fd);

    return 0;
}
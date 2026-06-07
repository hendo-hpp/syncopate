#include "network/tcp_listener.hpp"

#include "config/network.hpp"

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace syncopate::network {

tcp_listener::tcp_listener(uint16_t port) : m_port(port) {}

tcp_listener::~tcp_listener() { stop(); }

bool tcp_listener::start() {
    // communication endpoint
    m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_fd < 0) {
        std::cerr << "TCP_LISTENER: failed to create socket.\n";
        return false;
    }

    // socket address description
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind the address and port to the socket
    int bind_status = bind(m_server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr));
    if (bind_status < 0) {
        std::cerr << "TCP_LISTENER: failed to bind address to socket.\n";
        stop();
        return false;
    }

    // start listening on the socket, with a queue of length BACKLOG_COUNT
    int listen_status = listen(m_server_fd, syncopate::config::network::BACKLOG_COUNT);
    if (listen_status < 0) {
        std::cerr << "TCP_LISTENER: failed to begin listening on port " << m_port << ".\n";
        stop();
        return false;
    }

    m_is_listening = true;
    return true;
}

int tcp_listener::accept_connection() {
    if (!m_is_listening) {
        std::cerr << "TCP_LISTENER: socket is not listening for connection.\n";
        return -1;
    }

    // new socket to communicate on
    struct sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);

    int accepted_socket = accept(m_server_fd, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);
    if (accepted_socket < 0) {
        std::cerr << "TCP_LISTENER: failed to accept connection request.\n";
        return -1;
    }

    return accepted_socket;
}

void tcp_listener::stop() {
    if (m_server_fd == -1) {
        return;
    }

    close(m_server_fd);
    m_server_fd = -1;
    m_is_listening = false;
}

} // namespace syncopate::network
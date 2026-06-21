#include "network/tcp_listener.hpp"

#include "config/network.hpp"
#include "logger/logger.hpp"

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

namespace syncopate::network {

tcp_listener::tcp_listener(uint16_t port) : m_port(port) {}

tcp_listener::~tcp_listener() { stop(); }

int tcp_listener::start() {
    // communication endpoint
    m_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_fd < 0) {
        LOG_ERR("TCP_LISTENER: failed to create socket");
        return -1;
    }

    // allow socket to be reused instantly
    int opt = 1;
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        LOG_WARN("TCP_LISTENER: failed to set socket options");
    }

    // socket address description
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(m_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // bind the address and port to the socket
    if (bind(m_server_fd, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) < 0) {
        LOG_ERR("TCP_LISTENER: failed to bind address to socket");
        stop();
        return -1;
    }

    // start listening on the socket, with a queue of length BACKLOG_COUNT
    if (listen(m_server_fd, syncopate::config::network::BACKLOG_COUNT) < 0) {
        LOG_ERR("TCP_LISTENER: failed to begin listening on port " + std::to_string(m_port));
        stop();
        return -1;
    }

    m_is_listening = true;
    return 0;
}

int tcp_listener::accept_connection() {
    if (!m_is_listening) {
        LOG_ERR("TCP_LISTENER: socket is not listening for connection");
        return -1;
    }

    // new socket to communicate on
    struct sockaddr_in addr{};
    socklen_t addr_len = sizeof(addr);

    int client_fd = accept(m_server_fd, reinterpret_cast<struct sockaddr*>(&addr), &addr_len);
    if (client_fd < 0) {
        LOG_ERR("TCP_LISTENER: failed to accept connection request");
        return -1;
    }

    return client_fd;
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
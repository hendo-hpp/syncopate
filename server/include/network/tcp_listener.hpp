#ifndef SYNCOPATE_NETWORK_TCP_LISTENER_HPP__
#define SYNCOPATE_NETWORK_TCP_LISTENER_HPP__

#include <cstdint>
#include <string>

namespace syncopate::network {

class tcp_listener {
  public:
    explicit tcp_listener(uint16_t port);
    ~tcp_listener();

    // prevent copying of the listener
    tcp_listener(const tcp_listener&) = delete;
    tcp_listener& operator=(const tcp_listener&) = delete;

    int start();
    int accept_connection();
    void stop();

  private:
    uint16_t m_port;
    int m_server_fd{-1};
    bool m_is_listening{false};
};

} // namespace syncopate::network

#endif // SYNCOPATE_NETWORK_TCP_LISTENER_HPP__
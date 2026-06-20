#include "config/network.hpp"
#include "network/protocol_parser.hpp"
#include "network/tcp_listener.hpp"

#include <iostream>
#include <unistd.h>

int main() {
    syncopate::network::tcp_listener listener(syncopate::config::network::DEFAULT_PORT);
    if (listener.start() < 0) {
        std::cerr << "[TEST_SERVER]: failed to start listener.\n";
        return -1;
    }

    std::cout << "[TEST_SERVER]: server is listening on port 8080.\n";

    int client_fd = listener.accept_connection();
    if (client_fd < 0) {
        std::cerr << "[TEST_SERVER]: client failed to connect.\n";
        return -1;
    }

    std::cout << "[TEST_SERVER]: client has connected on fd: " << client_fd << ".\n";

    syncopate::network::protocol_parser protocol_parser;
    syncopate::network::session_context session_context;
    bool session_started = false;

    while (true) {
        syncopate::network::packet_type packet_type;
        if (protocol_parser.read_packet_type(client_fd, packet_type) < 0) {
            std::cout << "[TEST_SERVER]: connection lost/closed.\n";
            break;
        }

        if (packet_type == syncopate::network::packet_type::SESSION_START) {
            if (protocol_parser.parse_session_context(client_fd, session_context) < 0) {
                break;
            }
            session_started = true;
            std::cout << "[TEST_SERVER]: session started with "
                      << "source: " << static_cast<int>(session_context.source)
                      << ", dest: " << static_cast<int>(session_context.dest) << ".\n";
        }

        else if (packet_type == syncopate::network::packet_type::TRACK_PAYLOAD) {
            if (!session_started) {
                std::cerr << "[TEST_SERVER]: recieved payload before connection direction.\n";
                break;
            }
            syncopate::engine::track_task task;
            if (protocol_parser.parse_track_task(client_fd, task) < 0) {
                break;
            }
            std::cout << "[TEST_SERVER]: track recieved with "
                      << "name: " << task.track_name << ", artist: " << task.artist << ", album: " << task.album
                      << ".\n";
        }

        else if (packet_type == syncopate::network::packet_type::SESSION_END) {
            std::cout << "[TEST_SERVER]: session has ended successfully\n";
            break;
        }
    }

    close(client_fd);
    listener.stop();

    return 0;
}
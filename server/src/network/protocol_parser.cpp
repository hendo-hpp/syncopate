#include "network/protocol_parser.hpp"

#include "config/network.hpp"
#include "logger/logger.hpp"

#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <unistd.h>

namespace syncopate::network {

int protocol_parser::read_packet_type(int client_fd, packet_type& out_type) {
    uint8_t type_byte;
    if (read_bytes(client_fd, &type_byte, sizeof(type_byte)) < 0) {
        return -1;
    }
    out_type = static_cast<packet_type>(type_byte);
    return 0;
}

int protocol_parser::parse_session_context(int client_fd, session_context& out_context) {
    uint8_t source_byte;
    if (read_bytes(client_fd, &source_byte, sizeof(source_byte)) < 0) {
        return -1;
    }
    out_context.source = static_cast<engine::platform_type>(source_byte);

    uint8_t dest_byte;
    if (read_bytes(client_fd, &dest_byte, sizeof(dest_byte)) < 0) {
        return -1;
    }
    out_context.dest = static_cast<engine::platform_type>(dest_byte);

    return 0;
}

int protocol_parser::parse_track_task(int client_fd, engine::track_task& out_task) {
    // fields must be read in sequence
    if (read_string(client_fd, out_task.track_name) < 0) {
        return -1;
    }
    if (read_string(client_fd, out_task.artist) < 0) {
        return -1;
    }
    if (read_string(client_fd, out_task.album) < 0) {
        return -1;
    }
    if (read_string(client_fd, out_task.dest_playlist_id) < 0) {
        return -1;
    }
    if (read_string(client_fd, out_task.dest_auth_token) < 0) {
        return -1;
    }

    return 0;
}

int protocol_parser::read_bytes(int fd, void* dest, size_t target_size) {
    size_t bytes_consumed = 0;
    size_t bytes_remaining = 0;

    // for easy pointer arithmetic
    char* buffer_ptr = static_cast<char*>(dest);

    while (bytes_consumed < target_size) {
        bytes_remaining = target_size - bytes_consumed;

        ssize_t read_status = read(fd, buffer_ptr + bytes_consumed, bytes_remaining);
        if (read_status < 0) {
            LOG_ERR("PROTOCOL_PARSER: error reading from file descriptor: " + std::to_string(fd));
            return -1;
        }
        if (read_status == 0) {
            LOG_ERR("PROTOCOL_PARSER: client has disconnected on file descriptor: " + std::to_string(fd));
            return -1;
        }

        bytes_consumed += read_status;
    }

    return 0;
}

int protocol_parser::read_string(int fd, std::string& out_string) {
    uint16_t raw_string_length = 0;
    // read length prefix
    if (read_bytes(fd, &raw_string_length, sizeof(raw_string_length)) < 0) {
        return -1;
    }

    m_string_length = ntohs(raw_string_length);

    if (m_string_length == 0) {
        out_string.clear();
        return 0;
    }

    // resize buffer to fit the payload
    m_byte_buffer.resize(m_string_length);

    // read the string payload into the buffer
    if (read_bytes(fd, m_byte_buffer.data(), m_string_length) < 0) {
        return -1;
    }

    out_string.assign(m_byte_buffer.data(), m_string_length);
    return 0;
}

} // namespace syncopate::network
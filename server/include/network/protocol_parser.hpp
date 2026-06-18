#ifndef SYNCOPATE_NETWORK_PROTOCOL_PARSER_HPP__
#define SYNCOPATE_NETWORK_PROTOCOL_PARSER_HPP__

#include "engine/track.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace syncopate::network {

class protocol_parser {
  public:
    explicit protocol_parser() = default;
    ~protocol_parser() = default;

    int parse_track_task(int client_fd, engine::track_task& out_task);

  private:
    // parsing helpers
    int read_bytes(int fd, void* dest, size_t target_size);
    int read_string(int fd, std::string& out_string);

    // temporary data information, to stop allocating many buffers on the stack
    std::vector<char> m_byte_buffer;
    uint16_t m_string_length{0};
};

} // namespace syncopate::network

#endif // SYNCOPATE_NETWORK_TASK_PROTOCOL_PARSER_HPP__
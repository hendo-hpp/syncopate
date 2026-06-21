#ifndef SYNCOPATE_NETWORK_PACKET_HPP__
#define SYNCOPATE_NETWORK_PACKET_HPP__

#include "engine/platform.hpp"

#include <cstdint>

namespace syncopate::network {

enum class packet_type : uint8_t { SESSION_START, TRACK_PAYLOAD, SESSION_END };

struct session_context {
    engine::platform_type source;
    engine::platform_type dest;
};

} // namespace syncopate::network

#endif // SYNCOPATE_NETWORK_PACKET_HPP__
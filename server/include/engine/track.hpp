#ifndef SYNCOPATE_ENGINE_TRACK_HPP__
#define SYNCOPATE_ENGINE_TRACK_HPP__

#include <cstdint>
#include <string>

namespace syncopate::engine {

enum class platform_type : uint8_t {
    APPLE_MUSIC,
    SPOTIFY,
};

struct track_task {
    std::string track_name;
    std::string artist;
    std::string album;
    std::string dest_playlist_id;
    std::string dest_auth_token;
    platform_type source;
    platform_type dest;
};

} // namespace syncopate::engine

#endif // SYNCOPATE_ENGINE_TRACK_HPP__
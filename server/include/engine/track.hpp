#ifndef SYNCOPATE_ENGINE_TRACK_HPP__
#define SYNCOPATE_ENGINE_TRACK_HPP__

#include "engine/platform.hpp"

#include <string>

namespace syncopate::engine {

struct track_task {
    std::string track_name;
    std::string artist;
    std::string album;
    std::string dest_playlist_id;
    std::string dest_auth_token;

    // locations kept for worker threads to know
    // target APIs.
    platform_type source;
    platform_type dest;
};

} // namespace syncopate::engine

#endif // SYNCOPATE_ENGINE_TRACK_HPP__
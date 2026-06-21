#ifndef SYNCOPATE_ENGINE_PLATFORM_HPP__
#define SYNCOPATE_ENGINE_PLATFORM_HPP__

#include <cstdint>

namespace syncopate::engine {

enum class platform_type : uint8_t {
    APPLE_MUSIC,
    SPOTIFY,
};

} // namespace syncopate::engine

#endif // SYNCOPATE_ENGINE_PLATFORM_HPP__
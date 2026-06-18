#ifndef SYNCOPATE_CONFIG_NETWORK_HPP__
#define SYNCOPATE_CONFIG_NETWORK_HPP__

#include <cstdint>
#include <string_view>

namespace syncopate::config::network {

constexpr uint16_t DEFAULT_PORT = 8080;
constexpr int BACKLOG_COUNT = 20;

} // namespace syncopate::config::network

#endif // SYNCOPATE_CONFIG_NETWORK_HPP__
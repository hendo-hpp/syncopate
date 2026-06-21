#ifndef SYNCOPATE_NETWORK_SESSION_MANAGER_HPP__
#define SYNCOPATE_NETWORK_SESSION_MANAGER_HPP__

#include "engine/merger_queue.hpp"

namespace syncpate::network {

class session_manager {
  private:
    explicit session_manager(syncopate::engine::merger_queue& queue);
    ~session_manager() = default;

  public:
};

} // namespace syncpate::network

#endif // SYNCOPATE_NETWORK_SESSION_MANAGER_HPP__
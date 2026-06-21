#ifndef SYNCOPATE_ENGINE_MERGER_POOL_HPP__
#define SYNCOPATE_ENGINE_MERGER_POOL_HPP__

#include "engine/merger_queue.hpp"

#include <cstddef>
#include <pthread.h>
#include <vector>

namespace syncopate::engine {

class merger_pool {
  public:
    explicit merger_pool(merger_queue& queue, std::size_t num_threads);
    ~merger_pool();

    // prevent copying the thread pool
    merger_pool(const merger_pool&) = delete;
    merger_pool& operator=(const merger_pool&) = delete;

    int start();
    void stop();

  private:
    // thread environment setup
    static void* merger_thread_entry(void* arg);

    // work to actually be done by threads
    void merger_loop();

    merger_queue& m_queue;
    std::size_t m_num_threads;
    std::vector<pthread_t> m_threads;
    bool m_is_running{false};
};

} // namespace syncopate::engine

#endif // SYNCOPATE_ENGINE_MERGER_POOL_HPP__
#ifndef SYNCOPATE_ENGINE_MERGER_QUEUE_HPP__
#define SYNCOPATE_ENGINE_MERGER_QUEUE_HPP__

#include "engine/track.hpp"

#include <pthread.h>
#include <queue>
#include <cstddef>

namespace syncopate::engine {

class merger_queue {
  public:
    explicit merger_queue();
    ~merger_queue();

    void enqueue(const track_task& task);
    int dequeue(track_task& out_task);
    
    std::size_t size() const;
    
    void shutdown();

  private:
    std::queue<syncopate::engine::track_task> m_track_queue;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    bool m_server_shutdown{false};
};

} // namespace syncopate::engine

#endif // SYNCOPATE_ENGINE_MERGER_QUEUE_HPP__
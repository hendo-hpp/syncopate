#include "merger_queue.hpp"

#include <cstddef>
#include <pthread.h>

namespace syncopate::engine {

merger_queue::merger_queue() {
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

merger_queue::~merger_queue() {
    // mutex and condition variable must exist at this point, no checks needed
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

void merger_queue::enqueue(const track_task& task) {
    pthread_mutex_lock(&m_mutex);
    m_track_queue.push(task);
    pthread_mutex_unlock(&m_mutex);

    // signal to sleeping threads that a song has been added to queue
    pthread_cond_signal(&m_cond);
}

int merger_queue::dequeue(track_task& out_task) {
    pthread_mutex_lock(&m_mutex);

    // wrap the sleeping thread incase of spurious wake up
    while (m_track_queue.empty() && !m_server_shutdown) {
        pthread_cond_wait(&m_cond, &m_mutex);
    }

    // release resources on server shutdown
    if (m_server_shutdown && m_track_queue.empty()) {
        pthread_mutex_unlock(&m_mutex);
        return -1;
    }

    out_task = m_track_queue.front();
    m_track_queue.pop();

    // thread will aquire the mutex lock when woken up so it must release it when done
    pthread_mutex_unlock(&m_mutex);

    return 0;
}

std::size_t merger_queue::size() const {
    pthread_mutex_lock(&m_mutex);
    std::size_t curr_size = m_track_queue.size();
    pthread_mutex_unlock(&m_mutex);

    return curr_size;
}

void merger_queue::shutdown() {
    pthread_mutex_lock(&m_mutex);
    m_server_shutdown = true;
    pthread_mutex_unlock(&m_mutex);

    // propegate shutdown to other threads
    pthread_cond_broadcast(&m_cond);
}

} // namespace syncopate::engine

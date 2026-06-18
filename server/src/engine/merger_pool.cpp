#include "engine/merger_pool.hpp"

#include "config/engine.hpp"
#include "engine/track.hpp"

#include <cstddef>
#include <iostream>
#include <pthread.h>
#include <string>

// TODO: remove once API calls are ready, currently in use for testing
#include <chrono>
#include <thread>

namespace syncopate::engine {

merger_pool::merger_pool(merger_queue& queue, std::size_t num_threads) : m_queue(queue), m_num_threads(num_threads) {}

merger_pool::~merger_pool() { stop(); }

int merger_pool::start() {
    if (m_is_running) {
        return 0;
    }

    m_is_running = true;
    m_threads.resize(m_num_threads);

    for (std::size_t i = 0; i < m_num_threads; i++) {
        int ptc_result = pthread_create(&m_threads[i], nullptr, thread_entry, this);
        if (ptc_result != 0) {
            std::cerr << "MERGER_POOL: failed to create thread " << i << "\n";
            return -1;
        }
    }

    return 0;
}

void merger_pool::stop() {
    if (!m_is_running) {
        return;
    }

    m_is_running = false;
    m_queue.shutdown();

    for (std::size_t i = 0; i < m_num_threads; i++) {
        int ptj_result = pthread_join(m_threads[i], nullptr);
        if (ptj_result != 0) {
            std::cerr << "MERGER_POOL: failed to join thread " << i << "\n";
            return;
        }
    }
}

void* merger_pool::thread_entry(void* arg) {
    merger_pool* pool = static_cast<merger_pool*>(arg);
    pool->thread_loop();

    return nullptr;
}

void merger_pool::thread_loop() {
    track_task t_task;

    // consumers continuously look for work while the server is on
    while (true) {
        int dequeue_status = m_queue.dequeue(t_task);

        // stop work on shutdown
        if (dequeue_status < 0) {
            break;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        std::string log = "artist: " + t_task.artist + ". name: " + t_task.track_name + ".\n";
        std::cout << log;
    }
}

} // namespace syncopate::engine
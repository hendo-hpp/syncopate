#include "logger/logger.hpp"

#include <iostream>

namespace syncopate::logger {

async_logger& async_logger::instance() {
    static async_logger logger_instance;
    return logger_instance;
}

async_logger::async_logger() {
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}

async_logger::~async_logger() {
    stop();
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}

void async_logger::start() {
    if (m_is_running) {
        return;
    }

    m_is_running = true;
    pthread_create(&m_logging_thread, nullptr, logging_thread_entry, this);
}

void async_logger::stop() {
    if (!m_is_running) {
        return;
    }

    pthread_mutex_lock(&m_mutex);
    m_is_running = false;
    pthread_mutex_unlock(&m_mutex);

    pthread_cond_signal(&m_cond);
    pthread_join(m_logging_thread, nullptr);
}

void async_logger::log(const std::string& msg) {
    pthread_mutex_lock(&m_mutex);
    m_log_queue.push(msg);
    pthread_mutex_unlock(&m_mutex);
    pthread_cond_signal(&m_cond);
}

void* async_logger::logging_thread_entry(void* arg) {
    static_cast<async_logger*>(arg)->logging_loop();
    return nullptr;
}

void async_logger::logging_loop() {
    while (true) {
        pthread_mutex_lock(&m_mutex);

        while (m_log_queue.empty() && m_is_running) {
            pthread_cond_wait(&m_cond, &m_mutex);
        }

        if (!m_is_running && m_log_queue.empty()) {
            pthread_mutex_unlock(&m_mutex);
            break;
        }

        std::string msg = m_log_queue.front();
        m_log_queue.pop();
        pthread_mutex_unlock(&m_mutex);

        std::cout << msg << std::flush;
    }
}

} // namespace syncopate::logger

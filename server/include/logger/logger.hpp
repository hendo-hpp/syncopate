#ifndef SYNCOPATE_LOGGER_LOGGER_HPP__
#define SYNCOPATE_LOGGER_LOGGER_HPP__

#include <pthread.h>
#include <queue>
#include <string>

namespace syncopate::logger {

class async_logger {
  public:
    static async_logger& instance();

    void start();
    void stop();

    void log(const std::string& msg);

  private:
    async_logger();
    ~async_logger();

    static void* logging_thread_entry(void* arg);
    void logging_loop();

    std::queue<std::string> m_log_queue;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    pthread_t m_logging_thread;
    bool m_is_running{false};
};

#define LOG_INFO(msg) syncopate::logger::async_logger::instance().log("[INFO]: " + std::string(msg) + "\n");
#define LOG_WARN(msg) syncopate::logger::async_logger::instance().log("[WARN]: " + std::string(msg) + "\n");
#define LOG_ERR(msg) syncopate::logger::async_logger::instance().log("[ERROR]: " + std::string(msg) + "\n");

} // namespace syncopate::logger

#endif // SYNCOPATE_LOGGER_LOGGER_HPP__
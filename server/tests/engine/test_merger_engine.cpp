#include "engine/merger_pool.hpp"
#include "engine/merger_queue.hpp"
#include "engine/track.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

void correctness_test() {
    std::size_t test_thread_count = 4;

    std::cout << "[TEST]: initializing queue and pool.\n";
    syncopate::engine::merger_queue queue;
    syncopate::engine::merger_pool pool(queue, test_thread_count);

    int pool_start_result = pool.start();
    if (pool_start_result != 0) {
        std::cerr << "[TEST]: failed to start pool.\n";
        return;
    }

    for (int i = 0; i < 50; i++) {
        syncopate::engine::track_task task{"SONG " + std::to_string(i), "ARTIST NAME"};
        queue.enqueue(task);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "[TEST]: size of queue: " << queue.size() << "\n";

    pool.stop();
}

void speed_test() {
    const int num_benchmark_tasks = 200;

    // 4 thread benchmark
    {
        std::size_t test_thread_count = 4;

        syncopate::engine::merger_queue queue;
        syncopate::engine::merger_pool pool(queue, test_thread_count);

        for (int i = 0; i < num_benchmark_tasks; i++) {
            syncopate::engine::track_task task{"SONG " + std::to_string(i), "ARTIST NAME"};
            queue.enqueue(task);
        }

        auto start_time = std::chrono::high_resolution_clock::now();

        pool.start();
        pool.stop();

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        
        std::cout << "[BENCHMARK]: 4 threads took: " << duration << " ms\n";
    }
    {
        std::size_t test_thread_count = 1;

        syncopate::engine::merger_queue queue;
        syncopate::engine::merger_pool pool(queue, test_thread_count);

        for (int i = 0; i < num_benchmark_tasks; i++) {
            syncopate::engine::track_task task{"SONG " + std::to_string(i), "ARTIST NAME"};
            queue.enqueue(task);
        }

        auto start_time = std::chrono::high_resolution_clock::now();

        pool.start();
        pool.stop();

        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        
        std::cout << "[BENCHMARK]: 1 thread took: " << duration << " ms\n";
    }
}

int main() {
    // correctness_test();
    speed_test();

    return 0;
}
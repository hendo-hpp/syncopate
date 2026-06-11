add_library(sync_engine STATIC
    merger_queue.cpp
    merger_pool.cpp
)

target_include_directories(sync_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
add_library(sync_engine STATIC
    merger_pool.cpp    
    merger_queue.cpp
)

target_include_directories(sync_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
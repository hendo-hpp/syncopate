add_library(sync_logger STATIC
    logger.cpp
)

target_include_directories(sync_logger PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
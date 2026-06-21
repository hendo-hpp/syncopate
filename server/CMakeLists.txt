# build/project config
cmake_minimum_required(VERSION 3.23)
project(syncopate_server VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_subdirectory(src)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
    sync_network
    sync_engine
    sync_logger
    Threads::Threads
)


# custom format module
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(format)
enable_clang_format()


# testing config
option(BUILD_TESTS "Enable unit/integration testing" ON)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
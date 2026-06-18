add_library(sync_network STATIC
    protocol_parser.cpp
    tcp_listener.cpp
)

target_include_directories(sync_network PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/../../include
)
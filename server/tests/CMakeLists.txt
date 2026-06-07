add_executable(test_network_listener network/test_tcp_listener.cpp)

target_link_libraries(test_network_listener PRIVATE 
    sync_network
)
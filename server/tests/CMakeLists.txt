# tcp listener test
add_executable(test_network_listener network/test_tcp_listener.cpp)
target_link_libraries(test_network_listener PRIVATE sync_network)

# multithreaded merging test
add_executable(test_merger_engine engine/test_merger_engine.cpp)
target_link_libraries(test_merger_engine PRIVATE sync_engine pthread)
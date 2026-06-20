add_executable(test_protocol_parsing test_protocol_parsing.cpp)
target_link_libraries(test_protocol_parsing
    PRIVATE sync_network
)
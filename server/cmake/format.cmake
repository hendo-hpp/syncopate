function(enable_clang_format)
    find_program(CLANG_FORMAT_BIN "clang-format")

    if(CLANG_FORMAT_BIN)
        # gather all source and header files to format
        file(GLOB_RECURSE ALL_FORMAT_FILES 
            "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" 
            "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
        )

        # format target
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_FORMAT_FILES}
        )
    else()
        message(WARNING "clang-format bin not found. 'format' target unavailable")
    endif()
endfunction()
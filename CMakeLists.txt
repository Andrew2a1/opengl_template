cmake_minimum_required(VERSION 3.22)
project(opengl_template C CXX)

include(CTest)
enable_testing()

set(CONAN_TOOLCHAIN_FILENAME ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

if(NOT EXISTS ${CONAN_TOOLCHAIN_FILENAME})
    execute_process(
        COMMAND conan install ${CMAKE_SOURCE_DIR} -pr:b=default -s build_type=${CMAKE_BUILD_TYPE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMAND_ECHO STDOUT
    )
endif()

include(${CONAN_TOOLCHAIN_FILENAME})

add_subdirectory(src)
cmake_minimum_required(VERSION 3.22)
project(opengl_template C CXX)

include(cmake/conan.cmake)
setup_conan_toolchain()

include(CTest)
enable_testing()

include(${CONAN_TOOLCHAIN_FILENAME})
add_subdirectory(src)
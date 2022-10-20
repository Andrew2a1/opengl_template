cmake_minimum_required(VERSION 3.22)
project(opengl_template C CXX)

include(cmake/conan.cmake)
setup_conan_toolchain()

include(CTest)
enable_testing()

add_subdirectory(apps)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
cmake_minimum_required(VERSION 3.22)

find_package(FreeGLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    ${FreeGLUT_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${glm_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    FreeGLUT::freeglut_static
    GLEW::GLEW
    glm::glm
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED TRUE
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    add_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -O3)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
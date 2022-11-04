include(${CMAKE_SOURCE_DIR}/cmake/app.cmake)
project(app3 C CXX)

set(SOURCES
    src/main.cpp
    src/RightTriangle.cpp
)

set(HEADERS
    include/RightTriangle.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
setup_app(${PROJECT_NAME})

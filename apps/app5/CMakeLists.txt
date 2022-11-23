include(${CMAKE_SOURCE_DIR}/cmake/app.cmake)
project(app5 C CXX)

set(SOURCES
    src/main.cpp
    src/Texture.cpp
    src/Pendulum.cpp
)

set(HEADERS
    include/Texture.h
    include/Pendulum.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
setup_app(${PROJECT_NAME})

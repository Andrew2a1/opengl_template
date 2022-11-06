include(${CMAKE_SOURCE_DIR}/cmake/app.cmake)
project(app4 C CXX)

set(SOURCES
    src/main.cpp
    src/Spring.cpp
    src/Texture.cpp
)

set(HEADERS
    include/Spring.h
    include/Texture.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
configure_file("textures/wood.bmp" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/wood.bmp" COPYONLY)
setup_app(${PROJECT_NAME})

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
configure_file("textures/wood.bmp" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/wood.bmp" COPYONLY)
configure_file("textures/metal.bmp" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/metal.bmp" COPYONLY)
configure_file("textures/steel.bmp" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/steel.bmp" COPYONLY)
setup_app(${PROJECT_NAME})

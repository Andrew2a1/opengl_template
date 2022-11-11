include(${CMAKE_SOURCE_DIR}/cmake/app.cmake)
project(app4 C CXX)

set(SOURCES
    src/main.cpp
    src/Spring.cpp
    src/Texture.cpp
    src/Shader.cpp
)

set(HEADERS
    include/Spring.h
    include/Texture.h
    include/Shader.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
configure_file("textures/wood.bmp" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/wood.bmp" COPYONLY)
configure_file("shaders/texture.fs" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/texture.fs" COPYONLY)
configure_file("shaders/texture.vs" "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/texture.vs" COPYONLY)
setup_app(${PROJECT_NAME})

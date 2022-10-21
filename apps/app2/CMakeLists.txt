include(${CMAKE_SOURCE_DIR}/cmake/app.cmake)
project(app2 C CXX)

add_executable(${PROJECT_NAME} src/main.cpp)
setup_app(${PROJECT_NAME})

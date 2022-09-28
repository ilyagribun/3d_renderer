cmake_minimum_required(VERSION 3.19)
project(3d_renderer)

set(CMAKE_CXX_STANDARD 17)

include_directories(libs)
add_subdirectory(renderer)
add_executable(main main.cpp)

include_directories(/usr/local/include)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})

target_link_libraries(main renderer sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
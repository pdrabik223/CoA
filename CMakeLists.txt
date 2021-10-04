cmake_minimum_required(VERSION 3.19)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h plane/plane.cpp)

set(utility_dir
        utility/coord.h utility/coord.cpp)


add_executable(CoA
        CoA_main.cpp
        ${plane_dir}
        ${utility_dir})

add_subdirectory(SFML)

target_include_directories(CoA
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include)

target_link_directories(CoA
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src)

target_link_libraries(CoA
        sfml-graphics
        sfml-system
        sfml-window)


add_executable(plane_example
        plane/plane_example.cpp
        ${plane_dir}
        ${utility_dir})
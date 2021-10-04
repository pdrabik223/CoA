cmake_minimum_required(VERSION 3.20)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

add_executable(CoA
        CoA_main.cpp)

add_subdirectory(SFML)
add_subdirectory(plane)
add_subdirectory(utility)


target_include_directories(CoA
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include
        PUBLIC ${PROJECT_SOURCE_DIR}/plane
        PUBLIC ${PROJECT_SOURCE_DIR}/utility

        )

target_link_directories(CoA
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src
        PUBLIC ${PROJECT_SOURCE_DIR}/plane
        PUBLIC ${PROJECT_SOURCE_DIR}/utility
        )

target_link_libraries(CoA
        sfml-graphics
        sfml-system
        sfml-window
        plane_lib
        utility_lib
       )
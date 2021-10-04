cmake_minimum_required(VERSION 3.20)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

add_executable(CoA
        CoA_main.cpp)

add_subdirectory(SFML)


target_include_directories(CoA
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include
        )

target_link_directories(CoA
        PUBLIC sfml/src
        )

target_link_libraries(CoA
        sfml-graphics
        sfml-system
        sfml-window
       )
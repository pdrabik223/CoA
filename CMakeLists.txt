cmake_minimum_required(VERSION 3.19)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h plane/plane.cpp
        plane/cell_type.h)

set(utility_dir
        utility/coord.h utility/coord.cpp)

set(sample_dir
        sample_algorithm/sample.h sample_algorithm/sample.cpp
        sample_algorithm/cell.h sample_algorithm/cell.cpp)

set(random_walk_dir
        random_walk/random_walk_algorithm.h random_walk/random_walk_algorithm.cpp)

set(sfml_window_dir
        sfml_window/window.h sfml_window/window.cpp
        sfml_window/window_plane.h sfml_window/window_plane.cpp
        sfml_window/color_scheme.h sfml_window/color_scheme.cpp)


add_executable(CoA
        CoA_main.cpp
        ${plane_dir}
        ${utility_dir}
        ${sample_dir}
        ${random_walk_dir}
        ${sfml_window_dir})

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

add_executable(sample_example
        sample_algorithm/sample_example.cpp
        ${plane_dir}
        ${utility_dir}
        ${sample_dir}
        ${random_walk_dir}
        ${sfml_window_dir}
        )

target_include_directories(sample_example
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include)

target_link_directories(sample_example
        PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src)

target_link_libraries(sample_example
        sfml-graphics
        sfml-system
        sfml-window)

cmake_minimum_required(VERSION 3.19)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h plane/plane.cpp
        plane/cell_type.h
        maze/maze_generator.h maze/maze_generator.cpp
        utility/coord.h utility/coord.cpp)

set(sfml_window_dir
        sfml_window/window.h sfml_window/window.cpp
        sfml_window/window_plane.h sfml_window/window_plane.cpp
        sfml_window/color_scheme.h sfml_window/color_scheme.cpp)

set(sample_dir
        sample_algorithm/sample.h sample_algorithm/sample.cpp
        sample_algorithm/cell.h sample_algorithm/cell.cpp)

set(a_star_dir
        a_star/a_star.h a_star/a_star.cpp
        a_star/cell.h a_star/cell.cpp)
set(dijkstra_dir
        dijkstra/dijkstra.h dijkstra/dijkstra.cpp)

set(random_walk_dir
        random_walk/random_walk_algorithm.h random_walk/random_walk_algorithm.cpp)

set(maze_painter_dir
        maze_painter/maze_painter.h maze_painter/maze_painter.cpp)
add_subdirectory(SFML)

add_executable(CoA CoA_main.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${sample_dir}
        ${random_walk_dir}
        ${a_star_dir}
        ${dijkstra_dir})

add_executable(plane_example plane/plane_example.cpp
        ${plane_dir})

add_executable(sample_example sample_algorithm/sample_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${sample_dir}
        )
add_executable(random_walk_example random_walk/random_walk_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${random_wald_dir})

add_executable(a_star_example  a_star/a_star_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${a_star_dir})

add_executable(maze_painter  maze_painter/painter_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}

        ${random_walk_dir}
        ${a_star_dir}
        ${sample_dir}

        )

set(executables CoA a_star_example sample_example random_walk_example  maze_painter)

foreach(executable IN LISTS executables)
    target_include_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include)
    endforeach()

foreach(executable IN LISTS executables)
    target_link_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src)
endforeach()

foreach(executable IN LISTS executables)
    target_link_libraries(${executable}
            sfml-graphics
            sfml-system
            sfml-window)
endforeach()
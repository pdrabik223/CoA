cmake_minimum_required(VERSION 3.19)
project(CoA)

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h plane/plane.cpp
        plane/cell_type.h
        maze/maze_generator.h maze/maze_generator.cpp
        utility/coord.h utility/coord.cpp
        CoA_settings.h)

set(sfml_window_dir
        sfml_window/window.h sfml_window/window.cpp
        sfml_window/window_plane.h sfml_window/window_plane.cpp
        sfml_window/color_scheme.h sfml_window/color_scheme.cpp
        sfml_window/graph.h sfml_window/graph.cpp)

set(brute_force_dir
        brute_force/brute_force.h brute_force/brute_force.cpp
        brute_force/cell.h brute_force/cell.cpp)

set(dijkstra_dir
        dijkstra/dijkstra.h dijkstra/dijkstra.cpp
        dijkstra/cell.h dijkstra/cell.cpp)

set(random_walk_dir
        random_walk/random_walk_algorithm.h random_walk/random_walk_algorithm.cpp)

set(maze_painter_dir
        maze_painter/maze_painter.h maze_painter/maze_painter.cpp)

add_subdirectory(SFML)

add_executable(CoA CoA_main.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${brute_force_dir}
        ${random_walk_dir}
        ${dijkstra_dir}
        )

add_executable(plane_example plane/plane_example.cpp
        ${plane_dir})

add_executable(brute_force_example brute_force/brute_force_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${brute_force_dir}
        )
add_executable(random_walk_example random_walk/random_walk_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${random_wald_dir})

add_executable(dijkstra_example dijkstra/dijkstra_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${dijkstra_dir})

add_executable(maze_painter  maze_painter/painter_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}
        ${random_walk_dir}
        ${dijkstra_dir}
        ${brute_force_dir})

add_executable(maze_generator maze/maze_generator_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}
        ${random_walk_dir}
        ${dijkstra_dir}
        ${brute_force_dir})
add_executable(graph_example sfml_window/graph_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}
        ${random_walk_dir}
        ${dijkstra_dir}
        ${brute_force_dir})


set(executables CoA dijkstra_example brute_force_example random_walk_example maze_painter maze_generator graph_example)

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
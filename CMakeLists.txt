cmake_minimum_required(VERSION 3.19)
project(CoA)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/DEBUG)
endif ()

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h plane/plane.cpp
        plane/cell_type.h
        maze/maze_generator.h maze/maze_generator.cpp
        utility/coord.h utility/coord.cpp
        clock/clock.h clock/clock.cpp
        clock/tracer.h clock/tracer.cpp
        CoA_settings.h)

set(sfml_window_dir
        sfml_window/window.h sfml_window/window.cpp
        sfml_window/window_plane.h sfml_window/window_plane.cpp
        sfml_window/color_scheme.h sfml_window/color_scheme.cpp
        sfml_window/graph.h sfml_window/graph.cpp)

set(dijkstra_dir
        dijkstra/dijkstra.h dijkstra/dijkstra.cpp
        )

set(a_star_dir
        a_star/a_star.h a_star/a_star.cpp
        a_star/cell.h a_star/cell.cpp)

set(random_walk_dir
        random_walk/random_walk_algorithm.h random_walk/random_walk_algorithm.cpp)

set(maze_painter_dir
        maze_painter/maze_painter.h maze_painter/maze_painter.cpp)

set(clock_dir
        )

add_subdirectory(SFML)

add_executable(CoA CoA_main.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${dijkstra_dir}
        ${random_walk_dir}
        ${a_star_dir}
        )
add_executable(tests testing/testing.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${dijkstra_dir}
        ${random_walk_dir}
        ${a_star_dir}
        )
add_executable(plane_example plane/plane_example.cpp
        ${plane_dir})

add_executable(saved_maze saved_mazes/saved_mazes.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${random_walk_dir}
        ${a_star_dir}
        ${dijkstra_dir})

add_executable(dijkstra_example dijkstra/dijkstra_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${dijkstra_dir})
add_executable(random_walk_example random_walk/random_walk_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${random_wald_dir})

add_executable(a_star_example a_star/a_star_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${a_star_dir})

add_executable(maze_painter maze_painter/painter_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}
        ${random_walk_dir}
        ${a_star_dir}
        ${dijkstra_dir})

add_executable(maze_generator maze/maze_generator_example.cpp
        ${plane_dir}
        ${sfml_window_dir})

add_executable(graph_example sfml_window/graph_example.cpp
        ${plane_dir}
        ${sfml_window_dir})
add_executable(clock_example clock/clock_example.cpp
        ${plane_dir})

set(executables CoA a_star_example dijkstra_example random_walk_example maze_painter maze_generator graph_example saved_maze tests)

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
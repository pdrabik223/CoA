cmake_minimum_required(VERSION 3.19)
project(CoA)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/DEBUG)
endif ()

set(CMAKE_CXX_STANDARD 14)

set(plane_dir
        plane/plane.h
        plane/plane.cpp
        plane/cell_type.h
        maze/maze_generator.h
        maze/maze_generator.cpp
        utility/coord.h
        utility/coord.cpp
        CoA_settings.h)

set(sfml_window_dir
        sfml_window/window.h
        sfml_window/window.cpp
        sfml_window/window_plane.h
        sfml_window/window_plane.cpp
        sfml_window/color_scheme.h
        sfml_window/color_scheme.cpp
        sfml_window/graph.h
        sfml_window/graph.cpp)

set(path_search_dir
        path_search/dijkstra/dijkstra.h
        path_search/dijkstra/dijkstra.cpp
        path_search/a_star/a_star.h
        path_search/a_star/a_star.cpp
        path_search/cell.h
        path_search/cell.cpp
        path_search/random_walk/random_walk_algorithm.h
        path_search/random_walk/random_walk_algorithm.cpp
        path_search/right_hand_rule/right_hand_rule.h
        path_search/right_hand_rule/right_hand_rule.cpp
        path_search/depth_first/depth_first.h
        path_search/depth_first/depth_first.cpp
        path_search/greedy_best_first/greedy_best_first.h
        path_search/greedy_best_first/greedy_best_first.cpp
        path_search/random_step/random_step.h
        path_search/random_step/random_step.cpp
        path_search/graph_base.h
        path_search/graph_base.cpp
        path_search/path_search.h
        )

set(maze_painter_dir
        maze_painter/maze_painter.h maze_painter/maze_painter.cpp)

set(clock_dir
        )

add_subdirectory(SFML)
add_subdirectory(pm-stl)

add_executable(CoA CoA_main.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir}
        )
add_executable(tests testing/testing.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir}
        testing/pr_file_format.h
        testing/pr_file_format.cpp
        )
add_executable(plane_example plane/plane_example.cpp
        ${plane_dir})

add_executable(saved_maze saved_mazes/saved_mazes.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir})

add_executable(dijkstra_example path_search/dijkstra/dijkstra_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir})
add_executable(random_walk_example path_search/random_walk/random_walk_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir})

add_executable(a_star_example path_search/a_star/a_star_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${path_search_dir})

add_executable(maze_painter maze_painter/painter_example.cpp
        ${plane_dir}
        ${sfml_window_dir}
        ${maze_painter_dir}
        ${path_search_dir})

add_executable(maze_generator maze/maze_generator_example.cpp
        ${plane_dir}
        ${sfml_window_dir})

add_executable(graph_example sfml_window/graph_example.cpp
        ${plane_dir}
        ${sfml_window_dir})

set(executables CoA a_star_example dijkstra_example random_walk_example maze_painter maze_generator graph_example saved_maze tests)

foreach (executable IN LISTS executables)
    target_include_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/include
            PUBLIC ${PROJECT_SOURCE_DIR}/pm-stl
            )
endforeach ()

foreach(executable IN LISTS executables)
    target_link_directories(${executable}
            PUBLIC ${PROJECT_SOURCE_DIR}/sfml/src
            PUBLIC ${PROJECT_SOURCE_DIR}/pm-stl
            )
endforeach()

foreach(executable IN LISTS executables)
    target_link_libraries(${executable}
            sfml-graphics
            sfml-system
            sfml-window
            pm_stl
            )
endforeach()
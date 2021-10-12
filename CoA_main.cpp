//
// Created by piotr on 04/10/2021.
//
#include "brute_force/brute_force.h"
#include "dijkstra/dijkstra.h"
#include "maze/maze_generator.h"
#include "random_walk/random_walk_algorithm.h"
#include <iostream>
#define WINDOW_SIZE 500

#define RUN_BRUTEFORCE false
#define RUN_RANDOM_WALK false
#define RUN_DIJKSTRA true

#define USE_RANDOM_GEN true
#define USE_SQUARE_MAZE true
#define USE_CIRCULAR_MAZE true

void MessageMe(int maze_nr, const std::string &maze_type, const std::string &algorithm, size_t time, int path_length) {
  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}
int GenBruteForceVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    BruteForce cos(sic);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "random", "Brute Force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenDijkstraVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    Dijkstra cos(sic);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "random", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    RandomWalkAlgorithm cos(sic);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "random", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int GenBruteForceVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();
    BruteForce cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Brute force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenDijkstraVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    //    std::cout << "queue size: " << window.GetQueueSize();
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    RandomWalkAlgorithm cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int GenBruteForceVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();
    BruteForce cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "Brute force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenDijkstraVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    //    std::cout << "queue size: " << window.GetQueueSize();
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    RandomWalkAlgorithm cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int main() {

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

#if USE_RANDOM_GEN
#if RUN_BRUTEFORCE
  Window screen_1(Coord(0, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_1(GenBruteForceVisuals, std::ref(screen_1), color_scheme);
#endif

#if RUN_RANDOM_WALK
  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenRandomWalkVisuals, std::ref(screen_2), color_scheme);
#endif

#if RUN_DIJKSTRA
  Window screen_1(Coord(0, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_1(GenDijkstraVisuals, std::ref(screen_1), color_scheme);
#endif
#endif

#if USE_SQUARE_MAZE
#if RUN_BRUTEFORCE
  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenBruteForceVisualsMaze, std::ref(screen_2), color_scheme);
#endif

#if RUN_RANDOM_WALK
  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenRandomWalkVisualsMaze, std::ref(screen_2), color_scheme);
#endif

#if RUN_DIJKSTRA
  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenDijkstraVisualsMaze, std::ref(screen_2), color_scheme);
#endif
#endif

#if USE_CIRCULAR_MAZE
#if RUN_BRUTEFORCE
  Window screen_3(Coord(WINDOW_SIZE * 2, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_3(GenBruteForceVisualsCMaze, std::ref(screen_3), color_scheme);
#endif

#if RUN_RANDOM_WALK
  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenRandomWalkVisualsCMaze, std::ref(screen_2), color_scheme);
#endif

#if RUN_DIJKSTRA
  Window screen_3(Coord(WINDOW_SIZE * 2, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_3(GenDijkstraVisualsCMaze, std::ref(screen_3), color_scheme);
#endif
#endif

#if USE_SQUARE_MAZE or USE_CIRCULAR_MAZE or USE_RANDOM_GEN
#if RUN_BRUTEFORCE
  generator_1.join();

#endif

#if RUN_RANDOM_WALK
  generator_2.join();

#endif

#if RUN_DIJKSTRA
  generator_3.join();

#endif
#endif
  return 0;
}
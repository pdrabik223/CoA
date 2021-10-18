//
// Created by piotr on 04/10/2021.
//
#include "a_star/a_star.h"
#include "dijkstra/dijkstra.h"
#include "maze/maze_generator.h"
#include "random_walk/random_walk_algorithm.h"
#include <iostream>
#define WINDOW_SIZE 500

#define RUN_BRUTEFORCE true
#define RUN_RANDOM_WALK false
#define RUN_DIJKSTRA false

#define USE_RANDOM_GEN true
#define USE_SQUARE_MAZE false
#define USE_CIRCULAR_MAZE false

void MessageMe(int maze_nr, const std::string &maze_type, const std::string &algorithm, size_t time, int path_length) {
  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
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
    MessageMe(i, "random", "Brute Force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenAStarVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 40; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    AStar cos(sic);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "random", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    RandomWalk cos(sic);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "random", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int GenDijkstraVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();
    Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Brute force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenAStarVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    AStar cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    //    std::cout << "queue size: " << window.GetQueueSize();
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSquareMaze();

    RandomWalk cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int GenDijkstraVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();
    Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "Brute force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenAStarVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    AStar cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    //    std::cout << "queue size: " << window.GetQueueSize();
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsCMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenCircularMaze();

    RandomWalk cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "circular maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenDijkstraVisualsSMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSnailMaze();
    Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "snail maze", "Brute force", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenAStarVisualsSMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSnailMaze();

    AStar cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    //    std::cout << "queue size: " << window.GetQueueSize();
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "snail maze", "AStar", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsSMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenSnailMaze();

    RandomWalk cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "snail maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int main() {
  srand(time(NULL));
  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  Window screen_1(Coord(0, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_1(GenDijkstraVisualsSMaze, std::ref(screen_1), color_scheme);

  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_2(GenRandomWalkVisualsSMaze, std::ref(screen_2), color_scheme);

  Window screen_3(Coord(WINDOW_SIZE * 2, 0), WINDOW_SIZE, WINDOW_SIZE);
  std::thread generator_3(GenAStarVisualsSMaze, std::ref(screen_3), color_scheme);

  generator_1.join();
  generator_2.join();
  generator_3.join();

  return 0;
}
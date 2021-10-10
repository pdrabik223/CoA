//
// Created by piotr on 04/10/2021.
//
#include "brute_force/brute_force.h"
#include "dijkstra/dijkstra.h"
#include "maze/maze_generator.h"
#include "random_walk/random_walk_algorithm.h"
#include <iostream>
#define WINDOW_SIZE 500

int Loop(Window &window) {
  window.MainLoop();
  return 0;
}
void MessageMe(int maze_nr, std::string maze_type, std::string algorithm, size_t time, int path_length) {
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

    dijkstra::Dijkstra cos(sic);

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
    sic.GenMaze();
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
    sic.GenMaze();

    dijkstra::Dijkstra cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Dijkstra", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}
int GenRandomWalkVisualsMaze(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    MazeGenerator sic(100, 100);
    sic.GenMaze();

    RandomWalkAlgorithm cos(sic.GetPlane());

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    MessageMe(i, "square maze", "Random Walk", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());
  }
  return 420;
}

int main() {

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

    Window screen_1(Coord(0,0),WINDOW_SIZE, WINDOW_SIZE);

    Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);

    Window screen_3(Coord(WINDOW_SIZE*2, 0), WINDOW_SIZE, WINDOW_SIZE);

//  Window screen_4(Coord(0, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);
//
//  Window screen_5(Coord(WINDOW_SIZE, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);
//
//  Window screen_6(Coord(WINDOW_SIZE * 2, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);

    std::thread window_1(Loop, std::ref(screen_1));
    std::thread window_2(Loop, std::ref(screen_2));
    std::thread window_3(Loop, std::ref(screen_3));
//  std::thread window_4(Loop, std::ref(screen_4));
//  std::thread window_5(Loop, std::ref(screen_5));
//  std::thread window_6(Loop, std::ref(screen_6));

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::thread generator_1(GenRandomWalkVisuals, std::ref(screen_1), color_scheme);
    std::thread generator_2(GenBruteForceVisuals, std::ref(screen_2), color_scheme);
    std::thread generator_3(GenDijkstraVisuals, std::ref(screen_3), color_scheme);
//  std::thread generator_4(GenRandomWalkVisualsMaze, std::ref(screen_4), color_scheme);
//  std::thread generator_5(GenBruteForceVisualsMaze, std::ref(screen_5), color_scheme);
//  std::thread generator_6(GenDijkstraVisualsMaze, std::ref(screen_6), color_scheme);

    generator_1.join();
    generator_2.join();
    generator_3.join();
//  generator_4.join();
//  generator_5.join();
//  generator_6.join();

    window_1.join();
    window_2.join();
    window_3.join();
//  window_4.join();
//  window_5.join();
//  window_6.join();

  return 0;
}
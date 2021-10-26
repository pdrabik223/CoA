//
// Created by piotr on 12/10/2021.
//

#include "../a_star/a_star.h"
#include "../brute_force/brute_force.h"
#include "../random_walk/random_walk_algorithm.h"

#include <iostream>
#define WINDOW_SIZE 500
int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int GenBrutForceVisuals(Window &window, ColorScheme color_scheme, Plane maze) {

  BruteForce cos(maze);

  auto path = cos.FindPath(window, color_scheme);

  auto t1 = std::chrono::steady_clock::now();
  cos.FindPath();
  std::clog << "algorithm: BruteForce\ttime: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() << "ms\tpath length: " << path.size() << "\n";

  return 1;
}
int GenDijkstraVisuals(Window &window, ColorScheme color_scheme, Plane maze) {

  AStar cos(maze);

  auto path = cos.FindPath(window, color_scheme);

  auto t1 = std::chrono::steady_clock::now();
  cos.FindPath();
  std::clog << "algorithm: Dijkstra\ttime: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() << "ms\tpath length: " << path.size() << "\n";
  return 2;
}
int GenRandomWalkVisuals(Window &window, ColorScheme color_scheme, Plane maze) {

  RandomWalk cos(maze);

  auto path = cos.FindPath(window, color_scheme);

  auto t1 = std::chrono::steady_clock::now();
  cos.FindPath();
  std::clog << "algorithm: Random Walk\ttime: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() << "ms\tpath length: " << path.size() << "\n";

  return 3;
}

int main() {

  Plane cos("../saved_mazes/difference.txt");

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  Window screen_1(WINDOW_SIZE, WINDOW_SIZE);

  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);

  Window screen_3(Coord(0, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::thread generator_1(GenRandomWalkVisuals, std::ref(screen_1), color_scheme, cos);
  std::thread generator_2(GenBrutForceVisuals, std::ref(screen_2), color_scheme, cos);
  std::thread generator_3(GenDijkstraVisuals, std::ref(screen_3), color_scheme, cos);

  generator_1.join();
  generator_2.join();
  generator_3.join();

  return 0;
}
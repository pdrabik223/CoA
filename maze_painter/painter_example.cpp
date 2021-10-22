//
// Created by piotr on 09/10/2021.
//
#include "../path_search/path_search.h"

#include "../sfml_window/window.h"
#include "maze_painter.h"
#include <iostream>
#define WINDOW_SIZE 500
int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int GenBrutForceVisuals(Window &window, ColorScheme color_scheme, Plane maze) {

  Dijkstra cos(maze);

  auto path = cos.FindPath(window, color_scheme);

  auto t1 = std::chrono::steady_clock::now();
  cos.FindPath();
  std::clog << "algorithm: Dijkstra\ttime: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() << "ms\tpath length: " << path.size() << "\n";

  return 1;
}
int GenAStarVisuals(Window &window, ColorScheme color_scheme, Plane maze) {

  AStar cos(maze);

  auto path = cos.FindPath(window, color_scheme);

  auto t1 = std::chrono::steady_clock::now();
  cos.FindPath();
  std::clog << "algorithm: AStar\ttime: " << std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count() << "ms\tpath length: " << path.size() << "\n";
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

  MazePainter maze(800, 800, 20, 20);
  maze.MainLoop();
  maze.GetPlane().SaveToFile("../saved_mazes/test.txt");
  Plane cos("../saved_mazes/test.txt");

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  Window screen_1(WINDOW_SIZE, WINDOW_SIZE);

  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);

  Window screen_3(Coord(0, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);

  std::this_thread::sleep_for(std::chrono::milliseconds(500));

  std::thread generator_1(GenRandomWalkVisuals, std::ref(screen_1), color_scheme, cos);
  std::thread generator_2(GenBrutForceVisuals, std::ref(screen_2), color_scheme, cos);
  std::thread generator_3(GenAStarVisuals, std::ref(screen_3), color_scheme, cos);

  generator_1.join();
  generator_2.join();
  generator_3.join();

  //  system("pause");

  return 0;
}
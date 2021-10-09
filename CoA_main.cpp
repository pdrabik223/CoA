//
// Created by piotr on 04/10/2021.
//
#include "dijkstra/dijkstra.h"
#include "brute_force/brute_force.h"
#include "random_walk/random_walk_algorithm.h"
#include <iostream>
#define WINDOW_SIZE 500

int Loop(Window &window) {
  window.MainLoop();
  return 0;
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
    printf("maze nr: %d\t algorithm: BruteForce\t time: %lld ns\tpath length: %d\n", i, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());

    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 420;
}
int GenAStarVisuals(Window &window, ColorScheme color_scheme) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100, 10);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);

    dijkstra::Dijkstra cos(sic);

    auto path = cos.FindPath(window, color_scheme);
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("maze nr: %d\t algorithm: Dijkstra\t time: %lld ns\tpath length: %d\n", i, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());

    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));
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
    printf("maze nr: %d\t algorithm: Random Walk\t time: %lld ns\tpath length: %d\n", i, std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());

    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  return 420;
}
int main() {


  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  Window screen_1(WINDOW_SIZE, WINDOW_SIZE);

  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);

  Window screen_3(Coord(0, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);

  std::thread window_thread_1(Loop, std::ref(screen_1));
  std::thread window_thread_2(Loop, std::ref(screen_2));
  std::thread window_thread_3(Loop, std::ref(screen_3));

  std::this_thread::sleep_for(std::chrono::milliseconds (500));

  std::thread generator_1(GenRandomWalkVisuals, std::ref(screen_1), color_scheme);
  std::thread generator_2(GenBruteForceVisuals, std::ref(screen_2), color_scheme);
  std::thread generator_3(GenAStarVisuals, std::ref(screen_3), color_scheme);



  generator_1.join();
  generator_2.join();
  generator_3.join();

  window_thread_1.join();
  window_thread_2.join();
  window_thread_3.join();

  system("pause");
  return 0;
}
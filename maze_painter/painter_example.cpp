//
// Created by piotr on 09/10/2021.
//
#include "../a_star/a_star.h"
#include "../random_walk/random_walk_algorithm.h"
#include "../sample_algorithm/sample.h"
#include "../sfml_window/window.h"
#include "maze_painter.h"
#define WINDOW_SIZE 500
int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int GenSampleVisuals(Window &window, ColorScheme color_scheme,  Plane maze) {



    Sample cos(maze);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("algorithm: BruteForce\t time: %lld ns\tpath length: %d\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());



  return 1;
}
int GenAStarVisuals(Window &window, ColorScheme color_scheme,  Plane maze) {

    a_star::AStar cos(maze);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("algorithm: A*\t\t time: %lld ns\tpath length: %d\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());

return 2;

}
int GenRandomWalkVisuals(Window &window, ColorScheme color_scheme,  Plane maze) {

    RandomWalkAlgorithm cos(maze);

    auto path = cos.FindPath(window, color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("algorithm: Random Walk\t time: %lld ns\tpath length: %d\n", std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count(), path.size());


  return 3;
}


int main() {

  MazePainter maze(800,800,100,100);
  maze.MainLoop();
  Plane cos = maze.GetPlane();

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  Window screen_1(WINDOW_SIZE, WINDOW_SIZE);

  Window screen_2(Coord(WINDOW_SIZE, 0), WINDOW_SIZE, WINDOW_SIZE);

  Window screen_3(Coord(0, WINDOW_SIZE), WINDOW_SIZE, WINDOW_SIZE);

  std::thread window_thread_1(Loop, std::ref(screen_1));
  std::thread window_thread_2(Loop, std::ref(screen_2));
  std::thread window_thread_3(Loop, std::ref(screen_3));

  std::this_thread::sleep_for(std::chrono::milliseconds (500));

  std::thread generator_1(GenRandomWalkVisuals, std::ref(screen_1), color_scheme,cos);
  std::thread generator_2(GenSampleVisuals, std::ref(screen_2), color_scheme,cos);
  std::thread generator_3(GenAStarVisuals, std::ref(screen_3), color_scheme,cos);

  generator_1.join();
  generator_2.join();
  generator_3.join();

  window_thread_1.join();
  window_thread_2.join();
  window_thread_3.join();





  return 0;
}
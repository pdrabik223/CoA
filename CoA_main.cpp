//
// Created by piotr on 04/10/2021.
//
#include "plane/plane.h"
#include "sample_algorithm/sample.h"
#include "sfml_window/window.h"
#include "a_star/a_star.h"
#include <iostream>


int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int GenSampleVisuals(Window &window) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);



    Sample cos(sic);
    ColorScheme color_scheme;
    color_scheme.LoadRedSet();
    auto path = cos.FindPath(window,color_scheme);

    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("maze nr: %d\t algorithm: BruteForce\t time: %lld ms\n",i,std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count());


    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));


  }
  return 420;
}
int GenAStarVisuals(Window &window) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(100, 100);

    sic.SetCell({0, 0}, CellState::FINISH);
    sic.SetCell({50, 50}, CellState::START);


    a_star::AStar cos(sic);

    ColorScheme color_scheme;
    color_scheme.LoadOrangeSet();
    auto path = cos.FindPath(window,color_scheme);
    auto t1 = std::chrono::steady_clock::now();
    cos.FindPath();
    printf("maze nr: %d\t algorithm: A*\t time: %lld ns\n",i,std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - t1).count());

    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));


  }
  return 420;
}

int main() {
  srand(1);

  Window screen1(800, 800);
  Window screen2(800, 800);

  std::thread window_thread1(Loop, std::ref(screen1));
  std::thread generator1(GenSampleVisuals, std::ref(screen1));

  std::thread window_thread2(Loop, std::ref(screen2));
  std::thread generator2(GenSampleVisuals, std::ref(screen2));



  window_thread1.join();
  generator1.join();

  window_thread2.join();
  generator2.join();

  return 0;
}
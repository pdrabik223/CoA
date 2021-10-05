//
// Created by piotr on 04/10/2021.
//
#include "plane/plane.h"
#include "sample_algorithm/sample.h"
#include "sfml_window/window.h"
#include <iostream>

int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int main() {
  srand(time(NULL));
  Plane sic(10, 10);

  sic.SetCell({2, 2}, CellType::FINISH);

  sic.SetCell({6, 6}, CellType::START);

  sic.AddBorder();

  WindowPlane cos(sic);

  Window screen(800, 800);
  Window screen2(800, 800);

  std::thread window_thread(Loop, std::ref(screen));
  std::thread window_thread2(Loop, std::ref(screen2));

  for (int i = 0; i < 2200; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    screen.PushFrame(Plane(10, 10));
    screen2.PushFrame(Plane(10, 10));

  }

  window_thread.join();
  window_thread2.join();
  return 0;
}
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
int GenRandomVisuals(Window &window) {

  for (int i = 0; i < 2200; ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    window.PushFrame(Plane(200, 200));
  }
  return 420;
}

int GenVisuals(Window &window) {

  for (int i = 0; i < 10; ++i) {
    Plane sic(200, 200);

    sic.SetCell({0, 0}, CellType::FINISH);
    sic.SetCell({160, 160}, CellType::START);

    sic.SetCell({0, 160}, CellType::START);
    sic.SetCell({160, 0}, CellType::START);

    Sample cos(sic);
    ColorScheme color_scheme;
    color_scheme.LoadCyanSet();
    auto path = cos.FindPath(window,color_scheme);

    if(not path.empty()) window.SetWindowLabel("CoA - path found :)");
    window.SetWindowLabel("CoA - no path found :c");


    while (window.GetQueueSize() > 30)
      std::this_thread::sleep_for(std::chrono::seconds(1));


  }
  return 420;
}

int main() {
  srand(1);

  Window screen(800, 800);

  std::thread window_thread(Loop, std::ref(screen));
  std::thread generator1(GenVisuals, std::ref(screen));

  window_thread.join();
  generator1.join();

  return 0;
}
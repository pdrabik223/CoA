//
// Created by piotr on 11/10/2021.
//
#include "graph.h"

int Loop(Graph &window) {
  window.MainLoop();
  return 0;
}

int main() {
  Graph test(1000, 800);

  std::thread screen(Loop, std::ref(test));

  for (int i = 0; i < 600; i++) {
    test.PushData(i, i);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  screen.join();
  return 0;
}
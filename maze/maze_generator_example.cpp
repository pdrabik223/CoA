//
// Created by piotr on 10/10/2021.
//

#include "../sfml_window/window.h"
#include "maze_generator.h"
#include <iostream>

int Loop(Window &window) {
  window.MainLoop();
  return 0;
}

int main() {
  srand(time(NULL));
  Window screen(800, 800);

  //  screen.PushFrame(WindowPlane(100,100));

  MazeGenerator maze(100, 100);

  maze.GenSnailMaze();

  auto plane = maze.GetPlane();

  ConsoleDisplay(plane);

  WindowPlane screen_texture(plane);

  screen.PushFrame(WindowPlane(maze.GetPlane()));

  return 0;
}
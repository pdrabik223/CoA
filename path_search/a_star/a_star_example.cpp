//
// Created by piotr on 07/10/2021.
//
#include "../../plane/plane.h"
#include "a_star.h"
#include <iostream>
int main() {

  Plane sic(20, 20, 10);

  sic.AddBorder(CellState::WALL);
  sic.SetCell({1, 1}, CellState::FINISH);

  sic.SetCell({18, 18}, CellState::START);

  printf("original maze:\n");

  ConsoleDisplay(sic);

  AStar sample(sic);

  printf("\nsolution:\n");

  ConsoleDisplay(sic,sample.FindPath());

  //  ConsoleDisplay(sample.FindPath());

  getchar();

  return 0;
}
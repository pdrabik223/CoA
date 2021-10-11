//
// Created by piotr on 09/10/2021.
//
#include "../plane/plane.h"
#include "random_walk_algorithm.h"
int main() {

  Plane sic(20, 20, 10);

  sic.AddBorder(CellState::WALL);
  sic.SetCell({1, 1}, CellState::FINISH);

  sic.SetCell({18, 18}, CellState::START);

  printf("original maze:\n");

  ConsoleDisplay(sic);

  RandomWalkAlgorithm sample(sic);

  printf("\nsolution:\n");

  ConsoleDisplay(sic,sample.FindPath());


  getchar();

  return 0;
}
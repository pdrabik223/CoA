//
// Created by piotr on 07/10/2021.
//
#include "../brute_force/brute_force.h"
#include "../plane/plane.h"
#include "dijkstra.h"
#include <iostream>
int main() {

  Plane sic(20, 20, 10);

  sic.AddBorder(CellState::WALL);
  sic.SetCell({1, 1}, CellState::FINISH);

  sic.SetCell({18, 18}, CellState::START);

  printf("original maze:\n");

  ConsoleDisplay(sic);

  dijkstra::Dijkstra sample(sic);

  printf("\nsolution:\n");

  ConsoleDisplay(sic,sample.FindPath());

  ConsoleDisplay(sample.FindPath());

  getchar();

  return 0;
}
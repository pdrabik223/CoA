//
// Created by piotr on 07/10/2021.
//
#include "../plane/plane.h"
#include "../sample_algorithm/sample.h"
#include <iostream>
#include "a_star.h"
int main() {

  Plane sic(20, 20);

  sic.AddBorder();
  sic.SetCell({1, 1}, CellState::FINISH);

  sic.SetCell({18, 18}, CellState::START);

  printf("original maze:\n");

  ConsoleDisplay(sic);

  a_star::AStar sample(sic);

  printf("\nsolution:\n");


  ConsoleDisplay(sic,sample.FindPath());


  ConsoleDisplay(sample.FindPath());

  return 0;
}
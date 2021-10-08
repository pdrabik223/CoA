//
// Created by piotr on 07/10/2021.
//
#include "../plane/plane.h"
#include "../sample_algorithm/sample.h"
#include <iostream>
#include "a_star.h"
int main() {

  Plane sic(7, 7);

  sic.SetCell({1, 1}, CellState::FINISH);

  sic.SetCell({4, 4}, CellState::START);

  sic.AddBorder();


  printf("original maze:\n");

  ConsoleDisplay(sic);

  a_star::AStar sample(sic);

  printf("\nsolution:\n");

  ConsoleDisplay(sample.FindPath());
  char p;
  std::cin>>p;
  return 0;
}
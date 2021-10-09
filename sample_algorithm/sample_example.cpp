//
// Created by piotr on 05/10/2021.
//
#include "../plane/plane.h"
#include <iostream>
#include "sample.h"
int main() {

  Plane sic(10, 10, 10);

  sic.SetCell({2, 2}, CellState::FINISH);

  sic.SetCell({6, 6}, CellState::START);

  sic.AddBorder();


  printf("original maze:\n");

  ConsoleDisplay(sic);

  Sample sample(sic);

  printf("\nsolution:\n");

  ConsoleDisplay(sample.FindPath());

  return 0;
}
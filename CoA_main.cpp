//
// Created by piotr on 04/10/2021.
//
#include "plane/plane.h"
#include <iostream>
#include "sample_algorithm/sample.h"
int main() {

  Plane sic(10, 10);

  sic.SetCell({2, 2}, CellType::FINISH);

  sic.SetCell({6, 6}, CellType::START);

  sic.AddBorder();


  Sample sample(sic);


  return 0;
}
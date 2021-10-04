//
// Created by piotr on 05/10/2021.
//
#include "plane.h"


int main() {

  Plane sic(10, 10);

  sic.SetCell({2, 2}, Cell::FINISH);

  sic.SetCell({6, 6}, Cell::START);

  sic.AddBorder();

  ConsoleDisplay(sic);


  return 0;
}
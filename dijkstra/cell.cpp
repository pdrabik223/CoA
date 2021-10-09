//
// Created by piotr on 06/10/2021.
//

#include "cell.h"

double dijkstra::Cell::GetH() const {
  return h + g;
}

void dijkstra::Cell::SetFatherPtr(Cell &father) {
  Cell::father_ptr = &father;
  g = father.g + 1;
}

dijkstra::Cell::Cell(const dijkstra::Cell &other) {

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;
}
dijkstra::Cell &dijkstra::Cell::operator=(const dijkstra::Cell &other) {
  if (this == &other) return *this;

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;

  return *this;
}
unsigned dijkstra::Cell::GetG() const {
  return g;
}

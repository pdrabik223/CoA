//
// Created by piotr on 06/10/2021.
//

#include "cell.h"

double a_star::Cell::GetH() const {
  return h + g;
}

void a_star::Cell::SetFatherPtr(Cell &father) {
  Cell::father_ptr = &father;
  g = father.g + 1;
}

a_star::Cell::Cell(const a_star::Cell &other) {

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;
}
a_star::Cell &a_star::Cell::operator=(const a_star::Cell &other) {
  if (this == &other) return *this;

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;

  return *this;
}
unsigned a_star::Cell::GetG() const {
  return g;
}

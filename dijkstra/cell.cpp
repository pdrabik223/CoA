//
// Created by piotr on 06/10/2021.
//

#include "cell.h"

double Cell::GetH() const {
  return h ;
}

void Cell::SetFatherPtr(Cell &father) {
  Cell::father_ptr = &father;
  SetG(father.g + 1);
  //  father.SetSonPtr(*this);
}

Cell::Cell(const Cell &other) {

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;
}
Cell &Cell::operator=(const Cell &other) {
  if (this == &other) return *this;

  g = other.g;
  h = other.h;

//  father_ptr = other.father_ptr;// ?
  placement = other.placement;
  cell_type = other.cell_type;

  return *this;
}
unsigned Cell::GetG() const {
  return g;
}
void Cell::SetSonPtr(Cell &son) {

  Cell::son_ptr = &son;
}

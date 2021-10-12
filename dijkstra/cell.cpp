//
// Created by piotr on 06/10/2021.
//

#include "cell.h"

double Cell::GetH() const {
  return h;
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

  placement = other.placement;
  cell_type = other.cell_type;

  nodes = other.nodes;
  return *this;
}
unsigned Cell::GetG() const {
  return g;
}
Cell *Cell::GetSmallestG() {
  if (nodes.empty()) return nullptr;
  int smallest_g = 0;

  for (int i = 1; i < nodes.size(); i++) {
    if (nodes[i]->g < nodes[smallest_g]->g) smallest_g = i;
  }
  return nodes[smallest_g];
}
void Cell::Connect(Cell &new_cell) {
  //  if (not new_cell.is_connected)
  nodes.emplace_back(&new_cell);

  //  if (new_cell.g < g + 1)
  //
  //    SetG(new_cell.g + 1);
  //
  //  else if (new_cell.g > g + 1)
  //
  //    new_cell.SetG(g + 1);

  //    g = nodes.back()->g+1;
}

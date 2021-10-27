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

  nodes.emplace_back(&new_cell);
}
void Cell::UpdateG() {

  for (auto &node : nodes)
    if (node->g + 1 < g) g = node->g + 1;
}
void Cell::Clear() {
  if (cell_type == CellState::START)
    g = 0;
  else
    g = CELL_MAX;
}
void Cell::SetH(const double &h) { this->h = h; }
Cell::~Cell() {

  nodes.clear();
  for (auto &n : nodes) {
    delete n;
  }
}
Cell::Cell(CellState cell_type, const Coord &position) : cell_type(cell_type), placement(position) {

  if (cell_type == CellState::START) {
    g = 0;
  }
}

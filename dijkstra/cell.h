//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_CELL_H_
#define COA_A_STAR_CELL_H_

#include "../brute_force/cell.h"
#include "../plane/cell_type.h"
#include "../utility/coord.h"

#define CELL_MAX 100'000'000

namespace dijkstra {
struct Cell {
  Cell() : cell_type(CellState::EMPTY){};
  Cell(CellState cell_type, const Coord &position) : cell_type(cell_type), placement(position) {
    if (cell_type == CellState::START) father_ptr = nullptr;
  };
  Cell(const Cell &other);
  Cell &operator=(const Cell &other);

  void SetH(const double &h) { this->h = h; }
  void SetG(const unsigned int &g) {
    this->g = g;
    if (son_ptr)
      son_ptr->SetG(g + 1);
  }

  double GetH() const;

  unsigned GetG() const;

  /// beksa
  double GetF() const { return h; }

  void SetFatherPtr(Cell &father);
  void SetSonPtr(Cell &son);
  void ClearSonPtr() {
    if (son_ptr) {
      son_ptr->father_ptr = nullptr;
      son_ptr->ClearSonPtr();
      son_ptr = nullptr;
    }
  };
  Cell *GetFatherPtr() const { return father_ptr; };

  ~Cell() {
    delete father_ptr;
    delete son_ptr;
  }

  /// distance from cell to finish
  double h = 0;

  /// distance from cell to start
  unsigned g = 0;

  Cell *father_ptr = nullptr;
  Cell *son_ptr = nullptr;

  Coord placement;

  CellState cell_type;
};
}// namespace dijkstra
#endif//COA_A_STAR_CELL_H_

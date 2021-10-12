//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_CELL_H_
#define COA_A_STAR_CELL_H_

#include "../brute_force/cell.h"
#include "../plane/cell_type.h"
#include "../utility/coord.h"
#include <iostream>
#include <vector>

#define CELL_MAX 100'000'000
struct Cell {
  Cell() : cell_type(CellState::EMPTY){};
  Cell(CellState cell_type, const Coord &position) : cell_type(cell_type), placement(position) {
    if (cell_type == CellState::START) {
      g = 0;
    }
  };
  Cell(const Cell &other);
  Cell &operator=(const Cell &other);

  void SetH(const double &h) { this->h = h; }
  //  void SetG(const unsigned int &g) {
  //
  //
  //
  //      this->g = g;
  //      for (auto &n : nodes)
  //        if(n->g > g + 1 )n->SetG(g + 1);
  //
  //    got_g = true;
  //
  //    assert(nodes.size() <= 4);
  //  }
  void Clear() {
    //    nodes.clear();

    got_g = false;
    if (cell_type == CellState::START)
      g = 0;
    else
      g = 100'000'000;
  }

  void Connect(Cell &new_cell);

  double GetH() const;

  unsigned GetG() const;
  Cell *GetSmallestG();

  /// beksa
  double GetF() const { return h; }

  ~Cell() {
    nodes.clear();
    for (auto &n : nodes) {
      delete n;
    }
  }

  std::vector<Cell *> nodes;

  bool is_connected = false;
  bool got_g = false;

  /// distance from cell to finish
  double h = 100'000'000;

  /// distance from cell to start
  unsigned g = 100'000'000;
  Coord placement;
  CellState cell_type;
};

#endif//COA_A_STAR_CELL_H_

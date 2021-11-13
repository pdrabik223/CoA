//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_CELL_H_
#define COA_A_STAR_CELL_H_

#include "../plane/cell_type.h"
#include "../utility/coord.h"
#include <iostream>
#include <vector>

#define CELL_MAX 100'000'000

struct Cell {
  Cell() : cell_type(CellState::EMPTY){};
  Cell(CellState cell_type, const Coord &position);
  Cell(const Cell &other);
  Cell &operator=(const Cell &other);
  ~Cell();
  void Connect(Cell &new_cell);

  void Clear();
  double GetH() const;
  unsigned GetG() const;
  void UpdateG();
  /// returns pointer to neighbour with the smallest g a.k.a. distance from the start node
  Cell *GetSmallestG();
  double GetF() const { return h; }

  bool IsDiscovered() const { return g != CELL_MAX; };

  std::vector<Cell *> nodes;

  /// distance from cell to finish
  double h = CELL_MAX;
  /// distance from cell to start
  unsigned g = CELL_MAX;
  /// distance from cell to start + distance fom cell to finish
  unsigned f = CELL_MAX;

  Coord placement;
  CellState cell_type;
};

#endif//COA_A_STAR_CELL_H_

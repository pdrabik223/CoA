//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_CELL_H_
#define COA_A_STAR_CELL_H_

#include "../plane/cell_type.h"
#include "../sample_algorithm/cell.h"

namespace a_star {
struct Cell {
  Cell() : cell_type(CellType::EMPTY), distance(CELL_MAX){};
  Cell(CellType cell_type) : cell_type(cell_type), distance(CELL_MAX) {
    if (cell_type == CellType::START) distance = 0;
  };
  Cell(const Cell &other) = default;
  Cell &operator=(const Cell &other) = default;
  bool operator<(const Cell &rhs) const;
  bool operator>(const Cell &rhs) const;
  bool operator<=(const Cell &rhs) const;
  bool operator>=(const Cell &rhs) const;
  friend std::ostream &operator<<(std::ostream &os, const Cell &cell);

  float g;
  float f;

  size_t distance;

  CellType cell_type;

  };
}
#endif//COA_A_STAR_CELL_H_

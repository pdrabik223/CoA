//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_CELL_H_
#define COA_SAMPLE_ALGORITHM_CELL_H_
#include "../plane/cell_type.h"
#include <ostream>
#define CELL_MAX 100'000'000

struct Cell {
  Cell() : cell_type(CellType::EMPTY), distance(CELL_MAX){};
  Cell(CellType cell_type) : cell_type(cell_type), distance(CELL_MAX) {
    if (cell_type == CellType::START) distance = 0;
  };
  Cell(const Cell &other) = default;
  Cell &operator=(const Cell &other) = default;
  bool operator<(const Cell &rhs) const {

    return distance < rhs.distance;
  }
  bool operator>(const Cell &rhs) const {
    return rhs < *this;
  }
  bool operator<=(const Cell &rhs) const {
    return !(rhs < *this);
  }
  bool operator>=(const Cell &rhs) const {
    return !(*this < rhs);
  }
  friend std::ostream &operator<<(std::ostream &os, const Cell &cell) {
    switch (cell.cell_type) {

      case CellType::EMPTY: {
        if (cell.distance == CELL_MAX) {
          os << "  ";
        } else if (cell.distance < 10) {
          os << " " << cell.distance;
        } else if (cell.distance < 99) {
          os << cell.distance;
        } else {
          os << "--";
        }
      } break;
      case CellType::WALL: os << " #"; break;
      case CellType::START: os << " S"; break;
      case CellType::FINISH:
        os << " F";
        break;
    }

    return os;
  }

  size_t distance;
  CellType cell_type;
};

#endif//COA_SAMPLE_ALGORITHM_CELL_H_

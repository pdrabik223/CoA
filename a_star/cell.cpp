//
// Created by piotr on 06/10/2021.
//

#include "cell.h"
bool a_star::Cell::operator<(const a_star::Cell &rhs) const {

  return distance < rhs.distance;
}
bool a_star::Cell::operator>(const a_star::Cell &rhs) const {
  return rhs < *this;
}
bool a_star::Cell::operator<=(const a_star::Cell &rhs) const {
  return !(rhs < *this);
}
bool a_star::Cell::operator>=(const a_star::Cell &rhs) const {
  return !(*this < rhs);
}
std::ostream &a_star::operator<<(std::ostream &os, const a_star::Cell &cell) {
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

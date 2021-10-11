//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_CELL_H_
#define COA_SAMPLE_ALGORITHM_CELL_H_
#include "../plane/cell_type.h"
#include <ostream>
#define CELL_MAX 100'000'000

//struct Cell {
//  Cell() : cell_type(CellState::EMPTY), distance(CELL_MAX){};
//  Cell(CellState cell_type) : cell_type(cell_type), distance(CELL_MAX) {
//    if (cell_type == CellState::START) distance = 0;
//  };
//
//  Cell(const Cell &other) = default;
//  Cell &operator=(const Cell &other) = default;
//  bool operator<(const Cell &rhs) const;
//  bool operator>(const Cell &rhs) const;
//  bool operator<=(const Cell &rhs) const;
//  bool operator>=(const Cell &rhs) const;
//  friend std::ostream &operator<<(std::ostream &os, const Cell &cell);
//
//  size_t distance;
//  CellState cell_type;
//};

#endif//COA_SAMPLE_ALGORITHM_CELL_H_

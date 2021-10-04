//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_CELL_H_
#define COA_SAMPLE_ALGORITHM_CELL_H_

struct Cell {
  enum class CellType {
    EMPTY,
    WALL,
    START,
    FINISH
  };

  Cell(CellType cell_type):cell_type(cell_type),distance(0){};
  Cell(const Cell& other) = default;
  Cell& operator=(const Cell& other) = default;


  size_t distance;
  CellType cell_type;
};

#endif//COA_SAMPLE_ALGORITHM_CELL_H_

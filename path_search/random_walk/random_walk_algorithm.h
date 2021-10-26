//
// Created by piotr on 05/10/2021.
//

#ifndef COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_
#define COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_

#include "../graph_base.h"

class RandomWalk : public GraphBase {

 public:
  RandomWalk() = delete;
  /// constructs Sample object based of Plane
  RandomWalk(const Plane &other) : GraphBase(other){};
  RandomWalk(const RandomWalk &other) = default;
  RandomWalk &operator=(const RandomWalk &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~RandomWalk() override {
    ClearGraph();
  }

 private:

  Cell *PopRandomCell(std::vector<Cell *> &positions);

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

 protected:

  };

#endif//COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_

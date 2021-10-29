//
// Created by piotr on 29/10/2021.
//

#ifndef COA_PATH_SEARCH_RANDOMSTEP_H_
#define COA_PATH_SEARCH_RANDOMSTEP_H_

#include "../graph_base.h"
class RandomStep : public GraphBase {
  enum class Direction {
    UP,
    RIGHT,
    LEFT,
    DOWN
  };

 public:
  RandomStep() = delete;

  /// constructs Sample object based of Plane
  RandomStep(const Plane &other) : GraphBase(other){};
  RandomStep(const RandomStep &other) = default;
  RandomStep &operator=(const RandomStep &other) = default;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~RandomStep() override {
    ClearGraph();
  }

 private:
  Direction CalculateDirection(const Coord &c_1, const Coord &c_2);
  std::vector<Cell *> PopRandomPath(std::vector<Cell *> &positions);

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);
};

#endif//COA_PATH_SEARCH_RANDOMSTEP_H_

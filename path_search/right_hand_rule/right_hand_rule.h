//
// Created by piotr on 19/10/2021.
//

#ifndef COA_PATH_SEARCH_RIGHT_HAND_RULE_RHR_H_
#define COA_PATH_SEARCH_RIGHT_HAND_RULE_RHR_H_
#include "../graph_base.h"
class RHR : public GraphBase {
  enum class Direction {
    UP,
    RIGHT,
    LEFT,
    DOWN
  };

 public:
  explicit RHR(const Plane &other) : GraphBase(other){};

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;
  ~RHR() override {
    ClearGraph();
  }

 private:
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);
  bool UpdateGs();
  Cell *PopSuccessor(std::vector<Cell *> &open, Direction &current_direction);
  Direction CalculateDirection(const Coord &c_1, const Coord &c_2);
  RHR::Direction RotateRight(Direction direction);
  static RHR::Direction RotateLeft(Direction direction);
  static Direction CalculateDirectionAtBeginning(const Coord &c_1, const Coord &c_2);
};

#endif//COA_PATH_SEARCH_RIGHT_HAND_RULE_RHR_H_

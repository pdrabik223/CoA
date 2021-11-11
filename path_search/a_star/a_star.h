//
// Created by piotr on 06/10/2021.
//

#ifndef COA_PATH_SEARCH_A_STAR_A_STAR_H_
#define COA_PATH_SEARCH_A_STAR_A_STAR_H_
#include "../graph_base.h"
#include <vector>
class AStar : public GraphBase {

 public:
  AStar() = delete;
  /// constructs Sample object based of Plane
  explicit AStar(const Plane &other);

  AStar(const AStar &other) = default;

  AStar &operator=(const AStar &other) = default;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~AStar() override { ClearGraph(); };

 private:
  double EuclideanDistance(const Coord &position);

  /// finds Manhattan distance between position and closest finish point
  double ManhattanDistance(const Coord &position);

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

  Cell *PopSmallestH(std::vector<Cell *> &open_set);
};

#endif//COA_PATH_SEARCH_A_STAR_A_STAR_H_

//
// Created by piotr on 11/11/2021.
//

#ifndef COA_PATH_SEARCH_GREEDY_PDISTANCE_GREEDY_PDISTANCE_H_
#define COA_PATH_SEARCH_GREEDY_PDISTANCE_GREEDY_PDISTANCE_H_

#include "../graph_base.h"
#include <vector>
class GreedyPDistance : public GraphBase {
 public:
  GreedyPDistance() = delete;
  /// constructs Sample object based of Plane
  explicit GreedyPDistance(const Plane &other);

  GreedyPDistance(const GreedyPDistance &other) = default;

  GreedyPDistance &operator=(const GreedyPDistance &other) = default;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~GreedyPDistance() override { ClearGraph(); };

 private:
  double PDistance(const Coord &position);

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

  Cell *PopSmallestH(std::vector<Cell *> &open_set);
};

#endif//COA_PATH_SEARCH_GREEDY_PDISTANCE_GREEDY_PDISTANCE_H_

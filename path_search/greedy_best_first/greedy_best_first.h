//
// Created by piotr on 27/10/2021.
//

#ifndef COA_PATH_SEARCH_GREEDY_BEST_FIRST_GREEDY_BEST_FIRST_H_
#define COA_PATH_SEARCH_GREEDY_BEST_FIRST_GREEDY_BEST_FIRST_H_

#include "../graph_base.h"
class GreedyBestFirst : public GraphBase {

 public:
  GreedyBestFirst() = delete;
  GreedyBestFirst(const Plane &other, Neighbourhood neighbourhood);
  /// constructs Sample object based of Plane
  explicit GreedyBestFirst(const Plane &other) : GraphBase(other){};
  GreedyBestFirst(const GreedyBestFirst &other) = default;
  GreedyBestFirst &operator=(const GreedyBestFirst &other) = default;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~GreedyBestFirst() override { ClearGraph(); };

 private:
  double EuclideanDistance(const Coord &position);

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

  Cell *PopSmallestH(std::vector<Cell *> &open_set);
};

#endif//COA_PATH_SEARCH_GREEDY_BEST_FIRST_GREEDY_BEST_FIRST_H_

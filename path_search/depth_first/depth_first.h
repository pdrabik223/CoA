//
// Created by piotr on 27/10/2021.
//

#ifndef COA_PATH_SEARCH_DEPTH_FIRST_H_
#define COA_PATH_SEARCH_DEPTH_FIRST_H_

#include "../graph_base.h"
class DepthFirst : public GraphBase {

 public:
  DepthFirst() = delete;

  /// constructs Sample object based of Plane
  explicit DepthFirst(const Plane &other) : GraphBase(other){};
  DepthFirst(const DepthFirst &other) = default;
  DepthFirst &operator=(const DepthFirst &other) = default;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~DepthFirst() override {
    ClearGraph();
  }

 private:
  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

  Cell *PopDeepest(std::vector<Cell *> &open_set);
};

#endif//COA_PATH_SEARCH_DEPTH_FIRST_H_

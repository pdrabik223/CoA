//
// Created by piotr on 05/10/2021.
//

#ifndef COA_PATH_SEARCH_DIJKSTRA_DIJKSTRA_H_
#define COA_PATH_SEARCH_DIJKSTRA_DIJKSTRA_H_

#include "../graph_base.h"

/// for now there only can be one start and one finish cell
class Dijkstra : public GraphBase {
 public:
  Dijkstra() = delete;
  /// constructs Sample object based of Plane
  explicit Dijkstra(const Plane &other) : GraphBase(other){};
  Dijkstra(const Dijkstra &other) = default;
  Dijkstra &operator=(const Dijkstra &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() override;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) override;

  ~Dijkstra() override {
    ClearGraph();
  }

 private:

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

};


#endif//COA_PATH_SEARCH_DIJKSTRA_DIJKSTRA_H_

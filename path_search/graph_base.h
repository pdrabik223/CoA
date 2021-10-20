//
// Created by piotr on 20/10/2021.
//

#ifndef COA_PATH_SEARCH_GRAPH_BASE_H_
#define COA_PATH_SEARCH_GRAPH_BASE_H_

#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "cell.h"
#include <vector>
static void ConsoleDisplay(const std::vector<Coord> &path);
class GraphBase {
 public:
  GraphBase(const Plane &other);
  GraphBase(const GraphBase &other);
  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  virtual std::vector<Coord> FindPath() = 0;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  virtual std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) = 0;

  virtual ~GraphBase() { ClearGraph(); };

 protected:
  void GeneratePath();
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);
  void ClearGraph();
  void ConnectNeighbours(const Coord &position);
  Cell &GetCell(const Coord &position) { return this->copy_plane_[position.ToInt(width_)]; };

  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;
  std::vector<Coord> starting_points_;
  std::vector<Coord> final_points_;
  std::vector<Coord> shortest_path_;
};
static void ConsoleDisplay(const std::vector<Coord> &path) {
  if (path.size() > 15) {
    printf("path is to big, path size: %d\n", path.size());
    return;
  }
  if (path.empty()) {
    printf("there's no path\n");
    return;
  }
  for (int i = 0; i < path.size() - 1; i++)
    printf("(%d,%d) -> ", path[i].x, path[i].y);
  printf("(%d,%d)\n", path[path.size() - 1].x, path[path.size() - 1].y);
}
#endif//COA_PATH_SEARCH_GRAPH_BASE_H_

//
// Created by piotr on 05/10/2021.
//

#ifndef COA_DIJKSTRA_DIJKSTRA_H_
#define COA_DIJKSTRA_DIJKSTRA_H_
#include "../a_star/cell.h"
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "../sfml_window/window_plane.h"

static void ConsoleDisplay(const std::vector<Coord> &path);

/// for now there only can be one start and one finish cell
class Dijkstra {
 public:
  Dijkstra() = delete;
  /// constructs Sample object based of Plane
  Dijkstra(const Plane &other);
  Dijkstra(const Dijkstra &other);
  Dijkstra &operator=(const Dijkstra &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme);

  ~Dijkstra() {
    ClearGraph();
  }

 private:
  void ConnectNeighbours(const Coord &position);

  void GeneratePath();
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  Cell &GetCell(const Coord &position) { return copy_plane_[position.ToInt(width_)]; };

  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);
  void ClearGraph();

 protected:
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
  for (int i = 0; i < path.size() -1 ; i++)
    printf("(%d,%d) -> ", path[i].x, path[i].y);
  printf("(%d,%d)\n", path[path.size() -1].x, path[path.size() -1].y);

}

#endif//COA_DIJKSTRA_DIJKSTRA_H_

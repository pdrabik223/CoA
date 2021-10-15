//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_A_STAR_H_
#define COA_A_STAR_A_STAR_H_
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "cell.h"
#include <vector>

class Dijkstra {
  enum DistanceFunction {
    EUCLIDEAN_DISTANCE,
    MANHATTAN_DISTANCE
  };

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

  ~Dijkstra() { ClearGraph(); };

 private:
  void GeneratePath();

  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  /// finds Manhattan distance between position and closest finish point
  double EuclideanDistance(const Coord &position);

  double ManhattanDistance(const Coord &position);

  Cell &GetCell(const Coord &position) { return copy_plane_[position.ToInt(width_)]; };

  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

  void ClearGraph();
  void ConnectNeighbours(const Coord &position);
  bool UpdateGs();

  double ComputeH(Cell *target);

 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;

  std::vector<Coord> starting_points_;
  std::vector<Coord> final_points_;
  std::vector<Coord> shortest_path_;

  DistanceFunction used_distance_function_ = MANHATTAN_DISTANCE;
  Cell *PopSmallestH(std::vector<Cell *> &open_set);
};

#endif//COA_A_STAR_A_STAR_H_

//
// Created by piotr on 06/10/2021.
//

#ifndef COA_A_STAR_A_STAR_H_
#define COA_A_STAR_A_STAR_H_
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "cell.h"
#include <vector>

namespace a_star {
class AStar {
 public:
  AStar() = delete;
  /// constructs Sample object based of Plane
  AStar(const Plane &other);
  AStar(const AStar &other);
  AStar &operator=(const AStar &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme);

  virtual ~AStar();

 private:
  bool GenerateGraph();
  void GeneratePath();

  bool GenerateGraph(Window &window_handle, const ColorScheme &color_scheme);
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  bool SearchBreakingPoint(const std::vector<Coord>& possible_routs, bool& path_has_been_found);

  /// \param position center of returned "square"
  /// \return neighbouring cells to cell under specified position
  std::vector<Coord> GenNeighbours(const Coord &position);

  /// applies distance from cell to finish
  void ApplyHDistance(std::vector<Coord> &cells);

  /// searches for lowest g distance  in positions
  Coord PopBestFCell(std::vector<Coord> &positions);

  /// finds Manhattan distance between position and closest finish point
  double EuclideanDistance(const Coord &position);

  Cell& GetCell(const Coord& position){return copy_plane_[position.ToInt(width_)];};

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
}// namespace a_star
#endif//COA_A_STAR_A_STAR_H_

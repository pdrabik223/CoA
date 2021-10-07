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

 private:
  /// \param position center of returned "square"
  /// \return neighbouring cells to cell under specified position
  std::vector<Coord> GenNeighbours(const Coord &position);
  /// run GenNeighboursForAllPositions function for every point passed in positions param
  std::vector<Coord> GenNeighboursForAllPositions(const std::vector<Coord> &positions);

  std::vector<Coord> GenNeighboursButIgnoreDistance(const Coord& position);

  /// applies distance from starting cell
  void ApplyIteration(std::vector<Coord> &cells, const std::vector<Coord>& start_points);

  /// applies Manhattan distance to finish points
  void ApplyDistance(std::vector<Coord> &cells, const std::vector<Coord> &finish_points);

  /// searches for lowest distance in positions
  Coord GetBestCell(std::vector<Coord> &positions);

  /// searches for lowest g distance  in positions
  Coord GetBestGCell(std::vector<Coord> &positions);


  /// populate start point pram and finish point param with coordinates of start and finish
  void GetStartAndFinish(std::vector<Coord> &start_points, std::vector<Coord> &finish_points);

  /// finds Manhattan distance between position and closest finish point
  unsigned ManhattanDistance(const Coord& position,const std::vector<Coord> &finish_points);





 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;

};
}
#endif//COA_A_STAR_A_STAR_H_

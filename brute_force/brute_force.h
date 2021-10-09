//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_SAMPLE_H_
#define COA_SAMPLE_ALGORITHM_SAMPLE_H_
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "../sfml_window/window_plane.h"
#include "cell.h"


static void ConsoleDisplay(const std::vector<Coord> &path);

/// for now there only can be one start and one finish cell
class BruteForce {
 public:
  BruteForce() = delete;
  /// constructs Sample object based of Plane
  BruteForce(const Plane &other);
  BruteForce(const BruteForce &other);
  BruteForce &operator=(const BruteForce &other);

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
  void ApplyIteration(std::vector<Coord> &cells, unsigned iteration);
  /// searches for lowest distance in positions
  Coord GetBestCell(std::vector<Coord> &positions);
  /// populate start point pram and finish point param with coordinates of start and finish
  void GetStartAndFinish(std::vector<Coord> &start_points, std::vector<Coord> &finish_points);

 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;
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

#endif//COA_SAMPLE_ALGORITHM_SAMPLE_H_

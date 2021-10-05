//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_SAMPLE_H_
#define COA_SAMPLE_ALGORITHM_SAMPLE_H_
#include "../plane/plane.h"
#include "cell.h"



static void ConsoleDisplay(const std::vector<Coord> &path);

/// for now there only can be one start and one finish cell
class Sample {
 public:
  Sample() = delete;
  /// constructs Sample object based of Plane
  Sample(const Plane &other);
  Sample(const Sample &other);
  Sample &operator=(const Sample &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

 private:
  /// \param position center of returned "square"
  /// \return neighbouring cells to cell under specified position
  std::vector<Coord> GenNeighbours(const Coord &position);
  /// run GenNeighbours function for every point passed in positions param
  std::vector<Coord> GenNeighbours(const std::vector<Coord> &positions);
  /// checks for valid neighbours
  /// \param neighbours cells to check
  /// \return only valid steps
  std::vector<Coord> CheckNeighbours(std::vector<Coord> &neighbours);
  /// applies distance from starting cell
  void ApplyIteration(std::vector<Coord> &cells, unsigned iteration);
  /// searches for lowest distance in positions
  Coord GetBestCell(const std::vector<Coord> &positions);
  /// populate start point pram and finish point param with coordinates of start and finish
  void GetStartAndFinish(Coord& start_point,Coord& finish_point);

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
  for (int i = 0; i < path.size(); i++)

    printf("(%d,%d) -> ", path[i].x, path[i].y);
}

#endif//COA_SAMPLE_ALGORITHM_SAMPLE_H_

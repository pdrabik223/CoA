//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_SAMPLE_H_
#define COA_SAMPLE_ALGORITHM_SAMPLE_H_
#include "../plane/plane.h"
#include "cell.h"

static void ConsoleDisplay(const std::vector<Coord> &path);
class Sample {
 public:
  Sample() = delete;
  Sample(const Plane &other);
  Sample(const Sample &other);
  Sample &operator=(const Sample &other);

  std::vector<Coord> FindPath();

 private:
  std::vector<Coord> GenNeighbours(const Coord &position);
  std::vector<Coord> GenNeighbours(const std::vector<Coord> &positions);
  std::vector<Coord> CheckNeighbours(std::vector<Coord> &neighbours);
  void ApplyIteration(std::vector<Coord> &cells, unsigned iteration);
  Coord GetBestCell(const std::vector<Coord> &positions);

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
  for (int i = path.size() - 1; i >= 0; i--)
    printf("(%d,%d) -> ", path[i].x, path[i].y);
}

#endif//COA_SAMPLE_ALGORITHM_SAMPLE_H_

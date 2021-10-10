//
// Created by piotr on 08/10/2021.
//

#ifndef COA_MAZE_MAZE_GENERATOR_H_
#define COA_MAZE_MAZE_GENERATOR_H_

#include "../plane/plane.h"

struct SquareMazeInfo {
  unsigned hole_size = 1;
  Coord min_cavity_size = {5, 5};
  int min_cavity_area = 25;
};

class MazeGenerator {
  using Square = std::pair<Coord, Coord>;

 public:
  MazeGenerator() = delete;
  MazeGenerator(const int width, const int height) : plane_(width, height, 0){};
  MazeGenerator(const MazeGenerator &other) = default;
  MazeGenerator &operator=(const MazeGenerator &other) = default;

  void AddStartAndFinish();

  const Plane &GetPlane() const;

  void GenMaze() {
    plane_.Clear();
    plane_.AddBorder();
    RecursiveDivision({{0, 0}, {(int) plane_.GetWidth() - 1, (int) plane_.GetHeight() - 1}}, 0);
    AddStartAndFinish();
  };

 private:
  void RecursiveDivision(const Square &square, int depth);
  void DrawLine(const Coord &start, const Coord &finish, const Coord &breaking_point);

 protected:
  Plane plane_;
  SquareMazeInfo maze_info_;
};

#endif//COA_MAZE_MAZE_GENERATOR_H_

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

struct CircularMazeInfo {
  unsigned hole_size = 1;
  int wall_thickness = 1;
  int cavity_thickness = 1;
  int central_cavity_radius = 5;
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

  void GenSquareMaze() {
    plane_.Clear();
    plane_.AddBorder(CellState::WALL);
    RecursiveDivision({{0, 0}, {(int) plane_.GetWidth() - 1, (int) plane_.GetHeight() - 1}}, 0);
    AddStartAndFinish();
  };
  void GenCircularMaze() {
    plane_.Clear();
    RecursiveCircularDivision();
    Coord center = {(int) (plane_.GetWidth() / 2), (int) (plane_.GetHeight() / 2)};

    center.x += rand() % 3 - 1;
    center.y += rand() % 3 - 1;

    plane_.SetCell(center, CellState::FINISH);
    plane_.AddBorder(CellState::START);
  }
  void GenSnailMaze() {
    plane_.Fill(CellState::WALL);
    RecursiveSnake();
  }

 private:
  void RecursiveSnake();
  void RecursiveCircularDivision();
  void CheckBoundariesAndPush(const Coord &position, std::vector<Coord> &push_target);
  void RecursiveDivision(const Square &square, int depth);
  void DrawLine(const Coord &start, const Coord &finish, const Coord &breaking_point);
  void DrawLine(Coord start, Coord finish);

  void DrawCircle(const Coord &center, std::vector<Coord> &target, int radius);

 protected:
  Plane plane_;
  SquareMazeInfo maze_info_;
  CircularMazeInfo circular_maze_info_;
};

#endif//COA_MAZE_MAZE_GENERATOR_H_

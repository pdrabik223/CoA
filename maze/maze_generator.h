//
// Created by piotr on 08/10/2021.
//

#ifndef COA_MAZE_MAZE_GENERATOR_H_
#define COA_MAZE_MAZE_GENERATOR_H_

#include "../plane/plane.h"

enum class MazeType {
  EMPTY_PLANE,
  PLANE_5,
  PLANE_10,
  PLANE_20,
  PLANE_25,
  CIRCUlAR_MAZE,
  SQUARE_MAZE,
  SNAIL_MAZE
};

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
  MazeGenerator(const int width, const int height, MazeType maze_type) : plane_(width, height, 0) {

    switch (maze_type) {
      case MazeType::EMPTY_PLANE: GenRandomMaze(0); break;
      case MazeType::PLANE_5: GenRandomMaze(5); break;
      case MazeType::PLANE_10: GenRandomMaze(10); break;
      case MazeType::PLANE_20: GenRandomMaze(20); break;
      case MazeType::PLANE_25: GenRandomMaze(25); break;
      case MazeType::CIRCUlAR_MAZE: GenCircularMaze(); break;
      case MazeType::SQUARE_MAZE: GenSquareMaze(); break;
      case MazeType::SNAIL_MAZE: GenSnailMaze(); break;
    }
  };

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
  void GenRandomMaze(int infill_percentage) {

    for (int x = 0; x < plane_.GetWidth(); x++)
      for (int y = 0; y < plane_.GetHeight(); ++y) {
        if (rand() % 100 < infill_percentage)
          plane_.SetCell({x, y}, CellState::WALL);
        else
          plane_.SetCell({x, y}, CellState::EMPTY);
      }

    plane_.SetCell({(int) plane_.GetWidth() / 2, (int) plane_.GetHeight() / 2}, CellState::START);

    int x = rand() % 2 == 1 ? plane_.GetWidth() - 1 : 0;
    int y = rand() % 2 == 1 ? plane_.GetHeight() - 1 : 0;
    plane_.SetCell({x, y}, CellState::FINISH);
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

static std::string ToString(MazeType maze_type) {

  switch (maze_type) {
    case MazeType::EMPTY_PLANE: return "Empty plane";
    case MazeType::PLANE_5: return "Infill 5%";
    case MazeType::PLANE_10: return "Infill 10%";
    case MazeType::PLANE_20: return "Infill 20%";
    case MazeType::PLANE_25: return "Infill 25%";
    case MazeType::CIRCUlAR_MAZE: return "Circular maze";
    case MazeType::SQUARE_MAZE: return "Square maze";
    case MazeType::SNAIL_MAZE: return "Snail maze";
  }
}

#endif//COA_MAZE_MAZE_GENERATOR_H_

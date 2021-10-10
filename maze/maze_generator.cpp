//
// Created by piotr on 08/10/2021.
//

#include "maze_generator.h"
#include <iostream>

const Plane &MazeGenerator::GetPlane() const {
  return plane_;
}

void MazeGenerator::RecursiveDivision(const Square &square, int depth = 0) {

  if (depth > 10) return;

  if (square.first.x >= square.second.x) assert(false);
  if (square.first.y >= square.second.y) assert(false);

  const Coord kSquareSize(square.second.x - square.first.x, square.second.y - square.first.y);

  //  if (square.first.x >= square.second.x) return;
  //  if (square.first.y >= square.second.y) return;

  if (kSquareSize.x < maze_info_.min_cavity_size.x) return;
  if (kSquareSize.y < maze_info_.min_cavity_size.y) return;
  if (kSquareSize.x * kSquareSize.y < maze_info_.min_cavity_area) return;

  bool horizontal_line = (depth % 2) == 1;

  Coord line_start;
  Coord line_finish;
  Coord breaking_point;

  int loop_counter = 0;
  if (horizontal_line) {
    // y
    // /\
    // |
    // | A################
    // | #       1       #
    // | #               #
    // | S###############F
    // | #       2       #
    // | #               #
    // | ################B
    //  ------------------->x
    // A = square.first
    // B = square.second
    // S = line_start
    // F = line_finish
    // 1 = square_1
    // 2 = square_2

    line_start.x = square.first.x;
    line_finish.x = square.second.x;

    do {
      line_start.y = square.first.y + rand() % (kSquareSize.y - (maze_info_.min_cavity_size.y - 1)) + (maze_info_.min_cavity_size.y / 2);
      line_finish.y = line_start.y;
      if (plane_.GetCell(line_start) == CellState::WALL and plane_.GetCell(line_finish) == CellState::WALL) break;
      loop_counter++;
      if (loop_counter > 30) return;
    } while (1 < 2);

    breaking_point.y = line_start.y;
    breaking_point.x = line_start.x + 1 + rand() % (kSquareSize.x - 2);

  } else {
    // y
    // /\
    // |
    // | A#######S########
    // | #       #       #
    // | #       #       #
    // | #   1   #   2   #
    // | #       #       #
    // | #       #       #
    // | ########F#######B
    //  ------------------->x
    // A = square.first
    // B = square.second
    // S = line_start
    // F = line_finish
    // 1 = square_1
    // 2 = square_2

    line_start.y = square.first.y;
    line_finish.y = square.second.y;

    do {

      line_start.x = square.first.x + rand() % (kSquareSize.x - (maze_info_.min_cavity_size.x - 1)) + (maze_info_.min_cavity_size.x / 2);
      line_finish.x = line_start.x;

      if (plane_.GetCell(line_start) == CellState::WALL and plane_.GetCell(line_finish) == CellState::WALL) break;

      loop_counter++;
      if (loop_counter > 30) return;

    } while (1 < 2);

    breaking_point.x = line_start.x;//+ (rand() % (kSquareSize.x - 2)) + 1;
    breaking_point.y = line_start.y + 1 + rand() % (kSquareSize.y - 2);
  }
  Square square_1, square_2;

  square_1.first = square.first;
  square_2.second = square.second;

  square_1.second = line_finish;
  square_2.first = line_start;

  DrawLine(line_start, line_finish, breaking_point);

  const Coord kSquare1Size(square_1.second.x - square_1.first.x, square_1.second.y - square_1.first.y);
  if (!(kSquare1Size.x < maze_info_.min_cavity_size.x or kSquare1Size.y < maze_info_.min_cavity_size.y or kSquare1Size.x * kSquare1Size.y < maze_info_.min_cavity_area)) {

    RecursiveDivision(square_1, depth + 1);
  }

  const Coord kSquare2Size(square_2.second.x - square_2.first.x, square_2.second.y - square_2.first.y);
  if (!(kSquare2Size.x < maze_info_.min_cavity_size.x or kSquare2Size.y < maze_info_.min_cavity_size.y or kSquare2Size.x * kSquare2Size.y < maze_info_.min_cavity_area)) {
    RecursiveDivision(square_2, depth + 1);
  }
}

void MazeGenerator::RecursiveCircularDivision() {

  int max_radius = plane_.GetWidth() < plane_.GetHeight() ? plane_.GetWidth() / 2 : plane_.GetHeight() / 2;

  max_radius -= 2;// so the outer circle does not touch sides
  Coord center = {(int) (plane_.GetWidth() / 2), (int) (plane_.GetHeight() / 2)};
  for (int r = circular_maze_info_.central_cavity_radius; r < max_radius; r += circular_maze_info_.cavity_thickness + circular_maze_info_.wall_thickness + 1) {

    std::vector<Coord> circle;
    DrawCircle(center, circle, r);

    for (const auto &c : circle)
      plane_.SetCell(c, CellState::WALL);

    int hole_position = rand() % (circle.size() - 2);
    std::vector<Coord> neighbours;
    neighbours.emplace_back(circle[hole_position].x, circle[hole_position].y - 1);
    neighbours.emplace_back(circle[hole_position].x, circle[hole_position].y + 1);
    neighbours.emplace_back(circle[hole_position].x - 1, circle[hole_position].y);
    neighbours.emplace_back(circle[hole_position].x + 1, circle[hole_position].y);

    neighbours.emplace_back(neighbours.back().x + 1, neighbours.back().y);

    neighbours.emplace_back(neighbours.front().x, neighbours.front().y - 1);
    neighbours.emplace_back(neighbours.front().x, neighbours.front().y + 1);
    neighbours.emplace_back(neighbours.front().x - 1, neighbours.front().y);
    neighbours.emplace_back(neighbours.front().x + 1, neighbours.front().y);

    for (auto &n : neighbours)
      plane_.SetCell(n, CellState::EMPTY);
  }
}

void MazeGenerator::DrawLine(const Coord &start, const Coord &finish, const Coord &breaking_point) {

  assert((start.x != finish.x) xor (start.y != finish.y));
  assert((start.x != breaking_point.x) xor (start.y != breaking_point.y));
  assert((finish.x != breaking_point.x) xor (finish.y != breaking_point.y));

  //  assert((start.x != finish.x) and (start.y != finish.y));
  if (start.x == finish.x) {
    // horizontal line
    int line_size = finish.y - start.y;
    for (int p = start.y; p < finish.y; p++) {
      plane_.SetCell({start.x, p}, CellState::WALL);
    }
  } else if (start.y == finish.y) {
    // vertical line
    int line_size = finish.x - start.x;

    for (int p = start.x; p < finish.x; p++) {
      plane_.SetCell({p, start.y}, CellState::WALL);
    }
  } else
    throw "not a line";

  plane_.SetCell(breaking_point, CellState::EMPTY);
}
void MazeGenerator::AddStartAndFinish() {
  bool side = rand() % 2;

  while (1 < 2) {
    int start_x_shift = rand() % maze_info_.min_cavity_size.x;
    int start_y_shift = rand() % maze_info_.min_cavity_size.y;
    if (plane_.GetCell({start_x_shift, start_y_shift}) == CellState::EMPTY) {
      plane_.SetCell({start_x_shift, start_y_shift}, CellState::START);
      break;
    }
  }
  while (1 < 2) {
    int finish_x_shift = plane_.GetWidth() - 1 - rand() % maze_info_.min_cavity_size.x;
    int finish_y_shift = plane_.GetHeight() - 1 - rand() % maze_info_.min_cavity_size.y;

    if (plane_.GetCell({finish_x_shift, finish_y_shift}) == CellState::EMPTY) {
      plane_.SetCell({finish_x_shift, finish_y_shift}, CellState::FINISH);
      break;
    }
  }
}

void MazeGenerator::DrawCircle(const Coord &center, std::vector<Coord> &target, int radius) {
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    CheckBoundariesAndPush({center.y + y, center.x + x}, target);
    CheckBoundariesAndPush({center.y + x, center.x + y}, target);
    CheckBoundariesAndPush({center.y + x, center.x - y}, target);
    CheckBoundariesAndPush({center.y + y, center.x - x}, target);
    CheckBoundariesAndPush({center.y - y, center.x - x}, target);
    CheckBoundariesAndPush({center.y - x, center.x - y}, target);
    CheckBoundariesAndPush({center.y - x, center.x + y}, target);
    CheckBoundariesAndPush({center.y - y, center.x + x}, target);
    if (err <= 0) {
      ++y;
      err += 2 * y + 1;
    } else {
      --x;
      err -= 2 * x + 1;
    }
  }
}

void MazeGenerator::CheckBoundariesAndPush(const Coord &position, std::vector<Coord> &push_target) {
  if (position.x >= 0 and position.x < plane_.GetWidth() and position.y >= 0 and position.y < plane_.GetHeight()) {
    push_target.push_back(position);
  }
}

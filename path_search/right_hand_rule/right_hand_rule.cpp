//
// Created by piotr on 19/10/2021.
//

#include "right_hand_rule.h"
#include <array>
std::vector<Coord> RHR::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

std::vector<Coord> RHR::FindPath() {

  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}

bool RHR::UpdateGs() {
  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  Cell *q = &GetCell(starting_points_.back());

  open.push_back(q);

  Direction current_direction = CalculateDirectionAtBeginning(q->placement, final_points_[rand() % final_points_.size()]);

  while (not open.empty()) {

    q = PopSuccessor(open, current_direction);

    if (q != nullptr) {
      q->UpdateG();
      if (q->cell_type == CellState::FINISH) return true;
    }

    //    for (const auto kS : successors)
    //      open.push_back(kS);
    //    successors.clear();
    //
  }

  return false;
}

RHR::Direction RHR::RotateRight(Direction direction) {
  switch (direction) {

    case RHR::Direction::UP: return RHR::Direction::RIGHT;
    case RHR::Direction::LEFT: return RHR::Direction::UP;
    case RHR::Direction::RIGHT: return RHR::Direction::DOWN;
    case RHR::Direction::DOWN: return RHR::Direction::LEFT;
  }
}
RHR::Direction RHR::RotateLeft(Direction direction) {
  switch (direction) {

    case RHR::Direction::UP: return RHR::Direction::LEFT;
    case RHR::Direction::LEFT: return RHR::Direction::DOWN;
    case RHR::Direction::RIGHT: return RHR::Direction::UP;
    case RHR::Direction::DOWN: return RHR::Direction::RIGHT;
  }
}

RHR::Direction RHR::CalculateDirection(const Coord &c_1, const Coord &c_2) {
  RHR::Direction global_direction;
  if (c_1.x == c_2.x) {// c's are on the same vertical line
    if (c_1.y < c_2.y)
      global_direction = Direction::UP;
    else
      global_direction = Direction::DOWN;
  } else if (c_1.y == c_2.y) {
    if (c_1.x < c_2.x)
      global_direction = Direction::RIGHT;
    else
      global_direction = Direction::LEFT;
  } else
    throw "exception";

  return global_direction;
}

RHR::Direction RHR::CalculateDirectionAtBeginning(const Coord &c_1, const Coord &c_2) {
  RHR::Direction global_direction;

  if (c_1.x >= c_2.x) {// c_2 is on the left
    if (c_1.y <= c_2.y)
      global_direction = Direction::RIGHT;// left down
    else
      global_direction = Direction::UP;// left up
  } else {                             // is on the right
    if (c_1.y <= c_2.y)
      global_direction = Direction::DOWN;// right down
    else
      global_direction = Direction::LEFT;// right up
  }

  return global_direction;
}

Cell *RHR::PopSuccessor(std::vector<Cell *> &open, RHR::Direction &current_direction) {
  std::array<Cell *, 4> neighbours{nullptr, nullptr, nullptr, nullptr};
  Direction best_way = RotateLeft(RotateLeft(current_direction));

  if (open.back()->IsDiscovered())
    return Discovered(open, current_direction, neighbours, best_way);
  else
    return NotDiscovered(neighbours, best_way, open);
}
Cell *RHR::Discovered(std::vector<Cell *> &open, RHR::Direction &current_direction, std::array<Cell *, 4> &neighbours, RHR::Direction &best_way) {
  for (auto q : open.back()->nodes)
    if (!q->IsDiscovered())
      neighbours[(int) CalculateDirection(open.back()->placement, q->placement)] = q;

  open.pop_back();

  if (neighbours[(int) current_direction]) {
    best_way = current_direction;
  } else if (neighbours[(int) RotateRight(current_direction)]) {
    best_way = RotateRight(current_direction);
  } else if (neighbours[(int) RotateLeft(current_direction)]) {
    best_way = RotateLeft(current_direction);
  } else if (neighbours[(int) RotateLeft(RotateLeft(current_direction))]) {
    best_way = RotateLeft(RotateLeft(current_direction));
  } else
    return nullptr;

  AppendSurrounding(neighbours, open, best_way, current_direction);

  return open.back();
}
Cell *RHR::NotDiscovered(std::array<Cell *, 4> &neighbours, RHR::Direction &current_direction, std::vector<Cell *> &open) {
  Direction best_way = RotateLeft(RotateLeft(current_direction));
  auto q = open.back();
  open.pop_back();

  for (auto p : q->nodes)
    if (p->IsDiscovered()) {
      best_way = CalculateDirection(p->placement, q->placement);

      for (auto k : q->nodes)
        if (!k->IsDiscovered())
          neighbours[(int) CalculateDirection(q->placement, k->placement)] = k;

      AppendSurrounding(neighbours, open, best_way, current_direction);

      return q;
    }
  throw "exception";
}
void RHR::AppendSurrounding(const std::array<Cell *, 4> &neighbours, std::vector<Cell *> &open, RHR::Direction &best_way, Direction &current_direction) {

  if (neighbours[(int) RotateLeft(RotateLeft(best_way))])
    open.push_back(neighbours[(int) RotateLeft(RotateLeft(best_way))]);

  if (neighbours[(int) RotateLeft(best_way)])
    open.push_back(neighbours[(int) RotateLeft(best_way)]);

  if (neighbours[(int) RotateRight(best_way)])
    open.push_back(neighbours[(int) RotateRight(best_way)]);

  if (neighbours[(int) best_way])
    open.push_back(neighbours[(int) best_way]);

  current_direction = best_way;
}
bool RHR::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));


  Cell *q = &GetCell(starting_points_.back());

  open.push_back(q);

  Direction current_direction = CalculateDirectionAtBeginning(q->placement, final_points_[rand() % final_points_.size()]);

  while (not open.empty()) {

    q = PopSuccessor(open, current_direction);

    if (q != nullptr) {
      q->UpdateG();
      if (q->cell_type == CellState::FINISH) return true;
      HighlightPositions(window_handle, color_scheme, {q});
    }
  }

  return false;
}
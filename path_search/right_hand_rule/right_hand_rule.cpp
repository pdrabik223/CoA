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

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  Cell *q;
  Direction current_direction = CalculateDirection(open.back()->placement, final_points_.back());
  while (not open.empty()) {
    q = PopSuccessor(open, current_direction);
    {
      if (q != nullptr)
        if (not q->IsDiscovered()) {
          open.push_back(q);
          q->UpdateG();
          if (q->cell_type == CellState::FINISH) return true;
        }
    }
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
  } else {
    if (c_1.x < c_2.x)
      global_direction = Direction::LEFT;
    else
      global_direction = Direction::RIGHT;
  }

  return global_direction;
}

Cell *RHR::PopSuccessor(std::vector<Cell *> &open, RHR::Direction &current_direction) {

  std::array<Cell *, 4> neighbours{nullptr, nullptr, nullptr, nullptr};

  for (auto p : open.back()->nodes)
    if (!p->IsDiscovered())
      neighbours[(int) CalculateDirection(open.back()->placement, p->placement)] = p;

  open.pop_back();

  Direction best_way = RotateLeft(RotateLeft(current_direction));

  if (neighbours[(int) current_direction]) {
    best_way = current_direction;
  } else if (neighbours[(int) RotateRight(current_direction)]) {
    best_way = RotateRight(current_direction);
  } else if (neighbours[(int) RotateLeft(current_direction)]) {
    best_way = RotateLeft(current_direction);
  }

  for (int p = 0; p < neighbours.size(); ++p)
    if (neighbours[p] and p != (int) best_way)
      open.push_back(neighbours[p]);

  current_direction = best_way;
  return neighbours[(int) best_way];
}
bool RHR::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  std::vector<Cell *> successors;

  Cell *q = &GetCell(starting_points_.back());

  open.push_back(q);

  Direction current_direction = CalculateDirection(q->placement, final_points_[rand() % final_points_.size()]);

  while (not open.empty()) {
    if (q) {
      q->UpdateG();
      //          successors.push_back(q);
      if (q->cell_type == CellState::FINISH) return true;
    }

    HighlightPositions(window_handle, color_scheme, {q});
    q = PopSuccessor(open, current_direction);

    //    for (const auto kS : successors)
    //      open.push_back(kS);
    //    successors.clear();
    //
  }
  return false;
}
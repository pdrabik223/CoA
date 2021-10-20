//
// Created by piotr on 19/10/2021.
//

#include "right_hand_rule.h"
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
  Direction current_direction = Direction::UP;
  while (not open.empty()) {
    q = GenSuccessor(open.back(), current_direction);
    open.pop_back();
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

RHR::Direction RHR::RotateRight(RHR::Direction direction) {
  switch (direction) {

    case RHR::Direction::UP:
      return RHR::Direction::RIGHT;
    case RHR::Direction::LEFT: return RHR::Direction::UP;
    case RHR::Direction::RIGHT: return RHR::Direction::DOWN;
    case RHR::Direction::DOWN: return RHR::Direction::LEFT;
  }
}

RHR::Direction RHR::CalculateDirection(const Coord &c_1, const Coord &c_2, RHR::Direction orientation) {
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

  switch (orientation) {
    case Direction::LEFT:
      RotateRight(global_direction);
      RotateRight(global_direction);
      RotateRight(global_direction);
      break;
    case Direction::RIGHT:
      RotateRight(global_direction);
      break;
    case Direction::DOWN:
      RotateRight(global_direction);
      RotateRight(global_direction);
      break;
  }
  return global_direction;
}

Cell *RHR::GenSuccessor(Cell *cell, RHR::Direction &current_direction) {

  Cell *best_choice = nullptr;

  RHR::Direction best_choice_direction;
  // populate best_choice_direction with the worst option first
  switch (current_direction) {
    case Direction::UP:
      best_choice_direction = Direction::DOWN;
      break;
    case Direction::LEFT:
      best_choice_direction = Direction::RIGHT;
      break;
    case Direction::RIGHT:
      best_choice_direction = Direction::LEFT;
      break;
    case Direction::DOWN:
      best_choice_direction = Direction::UP;
      break;
  }

  for (auto p : cell->nodes) {
    Direction direction = CalculateDirection(cell->placement, p->placement, current_direction);
    if (direction > best_choice_direction) {
      best_choice_direction = direction;
      best_choice = p;
    }
  }

  current_direction = best_choice_direction;
  return best_choice;
}
bool RHR::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));
  std::vector<Cell *> successors;

  Cell *q;
  Direction current_direction = Direction::UP;
  while (not open.empty()) {
    q = GenSuccessor(open.back(), current_direction);
    open.pop_back();
    if (q != nullptr)

      for (const auto kP : q->nodes)
        if (not kP->IsDiscovered()) {
          successors.push_back(kP);
          kP->UpdateG();
          if (kP->cell_type == CellState::FINISH) return true;
        }

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    std::vector<Coord> highlighted_positions;
    highlighted_positions.reserve(successors.size());
    for (const auto kS : successors)
      highlighted_positions.push_back(kS->placement);
    highlights.HighlightCells(highlighted_positions);
    window_handle.PushFrame(highlights);

    for (const auto kS : successors)
      open.push_back(kS);

    successors.clear();
  }
  return false;
}

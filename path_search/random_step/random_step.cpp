//
// Created by piotr on 29/10/2021.
//

#include "random_step.h"
std::vector<Coord> RandomStep::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

std::vector<Coord> RandomStep::FindPath() {
  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}

bool RandomStep::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
  }
  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  std::vector<Cell *> successors;

  while (not open.empty()) {

    for (Cell *k_p : PopRandomPath(open))
      if (not k_p->IsDiscovered()) {
        k_p->UpdateG();
        if (k_p->cell_type == CellState::FINISH) return true;
        successors.push_back(k_p);
        open.push_back(k_p);
      }

    HighlightPositions(window_handle, color_scheme, successors);
    successors.clear();
  }
  return false;
}
std::vector<Cell *> RandomStep::PopRandomPath(std::vector<Cell *> &open) {
  Cell *q = open.back();

  std::vector<Cell *> path;
}
RandomStep::Direction RandomStep::CalculateDirection(const Coord &c_1, const Coord &c_2) {
  RandomStep::Direction global_direction;
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

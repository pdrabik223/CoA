//
// Created by piotr on 05/10/2021.
//

#include "random_walk_algorithm.h"
#include <algorithm>

std::vector<Coord> RandomWalk::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
std::vector<Coord> RandomWalk::FindPath() {
  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}

bool RandomWalk::UpdateGs() {
  std::vector<Cell *> open;
  open.reserve(width_ * height_);
  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
  }
  Cell *q;
  while (not open.empty()) {

    q = PopRandomCell(open);

    for (const auto kP : q->nodes)
      if (not kP->IsDiscovered()) {
        open.push_back(kP);
        kP->UpdateG();
        if (kP->cell_type == CellState::FINISH) return true;
      }
  }
  return false;
}

bool RandomWalk::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
  }
  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  std::vector<Cell *> successors;

  while (not open.empty()) {

    Cell *q = PopRandomCell(open);

    for (const auto kP : q->nodes)
      if (not kP->IsDiscovered()) {
        kP->UpdateG();
        if (kP->cell_type == CellState::FINISH) return true;
        successors.push_back(kP);
        open.push_back(kP);
      }

    HighlightPositions(window_handle, color_scheme, successors);
    successors.clear();
  }
  return false;
}
Cell *RandomWalk::PopRandomCell(std::vector<Cell *> &open_set) {

  int rand_pos = rand() % open_set.size();

  Cell *temp = open_set[rand_pos];
  open_set.erase(open_set.begin() + rand_pos);

  return temp;
}
RandomWalk::RandomWalk(const Plane &other, Neighbourhood neighbourhood) : GraphBase(other, neighbourhood) {}

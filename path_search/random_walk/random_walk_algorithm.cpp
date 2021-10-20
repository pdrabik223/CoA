//
// Created by piotr on 05/10/2021.
//

#include "random_walk_algorithm.h"
#include <algorithm>

RandomWalk &RandomWalk::operator=(const RandomWalk &other) {

  if (&other == this) return *this;

  width_ = other.width_;
  height_ = other.height_;
  copy_plane_.reserve(width_ * height_);

  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);

  for (const auto &f : other.starting_points_)
    starting_points_ = other.starting_points_;

  for (const auto &f : other.final_points_)
    final_points_ = other.final_points_;

  for (const auto &f : other.shortest_path_)
    shortest_path_ = other.shortest_path_;

  return *this;
}

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
Cell *RandomWalk::PopRandomCell(std::vector<Cell *> &open_set) {

  int rand_pos = rand() % open_set.size();

  Cell *temp = open_set[rand_pos];
  open_set.erase(open_set.begin() + rand_pos);

  return temp;
}

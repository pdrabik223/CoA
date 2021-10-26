//
// Created by piotr on 05/10/2021.
//

#include "dijkstra.h"

std::vector<Coord> Dijkstra::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
std::vector<Coord> Dijkstra::FindPath() {

  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}

bool Dijkstra::UpdateGs() {

  std::vector<Cell *> open;
  open.reserve(width_ * height_);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
  }

  std::vector<Cell *> successors;
  while (not open.empty()) {

    for (const auto &q : open)
      for (const auto kP : q->nodes)
        if (not kP->IsDiscovered()) {
          successors.push_back(kP);
          kP->g = q->g + 1;
          if (kP->cell_type == CellState::FINISH) return true;
        }

    open = successors;
    successors.clear();
  }
  return false;
}
bool Dijkstra::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
  }

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  std::vector<Cell *> successors;
  while (not open.empty()) {

    for (const auto &q : open)
      for (const auto kP : q->nodes) {
        if (not kP->IsDiscovered()) {
          successors.push_back(kP);

          kP->g = q->g + 1;

          if (kP->cell_type == CellState::FINISH) return true;
        }
      }

    HighlightPositions(window_handle, color_scheme, successors);

    open = successors;
    successors.clear();
  }
  return false;
}
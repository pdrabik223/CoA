//
// Created by piotr on 27/10/2021.
//

#include "depth_first.h"
std::vector<Coord> DepthFirst::FindPath() {
  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}
std::vector<Coord> DepthFirst::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
bool DepthFirst::UpdateGs() {
  std::vector<Cell *> open;
  open.reserve(width_ * height_);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  Cell *q;

  while (not open.empty()) {

    q = PopDeepest(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        k_p->UpdateG();
        if (k_p->cell_type == CellState::FINISH) return true;
        open.push_back(k_p);
      }
  }
  return false;
}
Cell *DepthFirst::PopDeepest(std::vector<Cell *> &open_set) {
  int biggest_g = 0;

  for (int i = 1; i < open_set.size(); i++)
    if (open_set[i]->g > open_set[biggest_g]->g)
      biggest_g = i;

  Cell *temp = open_set[biggest_g];
  open_set.erase(open_set.begin() + biggest_g);
  return temp;
}
bool DepthFirst::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Cell *> open;
  open.reserve(width_ * height_);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));
  std::vector<Cell *> successors;

  Cell *q;

  while (not open.empty()) {

    q = PopDeepest(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        successors.push_back(k_p);
        k_p->UpdateG();
        if (k_p->cell_type == CellState::FINISH) return true;
        open.push_back(k_p);
      }

    //    for (const auto kS : successors)
    //      open.push_back(kS);

    HighlightPositions(window_handle, color_scheme, successors);

    successors.clear();
  }
  return false;
}

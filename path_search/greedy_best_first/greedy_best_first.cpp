//
// Created by piotr on 05/10/2021.
//

#include "greedy_best_first.h"
#include "pm_include.h"
#include <algorithm>



std::vector<Coord> GreedyBestFirst::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

bool GreedyBestFirst::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));
  std::vector<Cell *> successors;

  while (not open.empty()) {

    Cell *q = PopSmallestH(open);

    for (const auto kP : q->nodes)
      if (not kP->IsDiscovered()) {
        kP->UpdateG();
        kP->h = EuclideanDistance(kP->placement);
        if (kP->cell_type == CellState::FINISH) return true;
        successors.push_back(kP);
      }

    HighlightPositions(window_handle, color_scheme, successors);

    for (const auto kS : successors)
      open.push_back(kS);

    successors.clear();
  }
  return false;
}

std::vector<Coord> GreedyBestFirst::FindPath() {

  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}
bool GreedyBestFirst::UpdateGs() {

  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  Cell *q;

  while (not open.empty()) {
    q = PopSmallestH(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        k_p->UpdateG();
        k_p->h = EuclideanDistance(k_p->placement);
        if (k_p->cell_type == CellState::FINISH) return true;
        open.push_back(k_p);
      }
  }

  return false;
}

//
//double GreedyBestFirst::EuclideanDistance(const Coord &position) {
//
//  double smallest_distance = 100000000;
//
//  for (auto &f : final_points_) {
//    double distance = pow(position.x - f.x, 2) + pow(position.y - f.y, 2);
//    if (distance < smallest_distance) smallest_distance = distance;
//  }
//
//  return smallest_distance;
//}

Cell *GreedyBestFirst::PopSmallestH(std::vector<Cell *> &open_set) {

  int smallest_h = 0;

  for (int i = 1; i < open_set.size(); i++)
    if (open_set[i]->h < open_set[smallest_h]->h)
      smallest_h = i;

  Cell *temp = open_set[smallest_h];
  open_set.erase(open_set.begin() + smallest_h);
  return temp;
}
GreedyBestFirst::GreedyBestFirst(const Plane &other, Neighbourhood neighbourhood) : GraphBase(other, neighbourhood) {}

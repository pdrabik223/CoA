//
// Created by piotr on 05/10/2021.
//

#include "a_star.h"
#include "pm_include.h"
#include <algorithm>

AStar::AStar(const Plane &other) : GraphBase(other) {}
AStar::AStar(const Plane &other, Neighbourhood neighbourhood) : GraphBase(other, neighbourhood) {
}
std::vector<Coord> AStar::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

std::vector<Coord> AStar::FindPath() {

  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}

bool AStar::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));
  std::vector<Cell *> successors;

  while (not open.empty()) {

    Cell *q = PopSmallestF(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        k_p->UpdateG();
        k_p->h = ManhattanDistance(k_p->placement);
        k_p->f = k_p->h + k_p->h;
        if (k_p->cell_type == CellState::FINISH) return true;
        open.push_back(k_p);
        successors.push_back(k_p);
      }

    HighlightPositions(window_handle, color_scheme, successors);

    successors.clear();
  }
  return false;
}
bool AStar::UpdateGs() {

  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  Cell *q;

  while (not open.empty()) {
    q = PopSmallestF(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        k_p->UpdateG();
        k_p->h = ManhattanDistance(k_p->placement);
        k_p->f = k_p->h + k_p->h;
        if (k_p->cell_type == CellState::FINISH) return true;
        open.push_back(k_p);
      }
  }

  return false;
}
//
//double AStar::EuclideanDistance(const Coord &position) {
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
//
//double AStar::ManhattanDistance(const Coord &position) {
//  if (final_points_.size() == 1)
//    return Abs(position.x - final_points_.back().x) + Abs(position.y - final_points_.back().y);
//
//  double smallest_distance = 100000000;
//
//  for (auto &f : final_points_) {
//    double distance = Abs(position.x - f.x) + Abs(position.y - f.y);
//    if (distance < smallest_distance) smallest_distance = distance;
//  }
//
//  return smallest_distance;
//}

Cell *AStar::PopSmallestF(std::vector<Cell *> &open_set) {

  int smallest_f = 0;
  //    unsigned max_g = open_set.back()->g;
  //    unsigned max_h = 100000;
  //    for (auto h : open_set)
  //      if (h->h < max_h) max_h = h->h;

  for (int i = 1; i < open_set.size(); i++) {
    if (open_set[i]->f < open_set[smallest_f]->f) {
      smallest_f = i;
    }
  }

  Cell *temp = open_set[smallest_f];
  open_set.erase(open_set.begin() + smallest_f);
  return temp;
}

//
// Created by piotr on 05/10/2021.
//

#include "a_star.h"
#include "pm_include.h"
#include <algorithm>

int Abs(int x) {
  if (x < 0) return -x;
  else
    return x;
}

AStar::AStar(const Plane &other) : GraphBase(other) {}

AStar::AStar(const AStar &other) : GraphBase(other) {}

std::vector<Coord> AStar::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not UpdateGs(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

bool AStar::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));
  std::vector<Cell *> successors;

  while (not open.empty()) {

    Cell *q = PopSmallestH(open);

    for (const auto kP : q->nodes)
      if (not kP->IsDiscovered()) {
        successors.push_back(kP);
        kP->UpdateG();
        kP->h = EuclideanDistance(kP->placement);
        if (kP->cell_type == CellState::FINISH) return true;
      }

    HighlightPositions(window_handle, color_scheme, successors);

    for (const auto kS : successors)
      open.push_back(kS);

    successors.clear();
  }
  return false;
}

std::vector<Coord> AStar::FindPath() {

  ClearGraph();

  if (not UpdateGs()) return {};

  GeneratePath();

  return shortest_path_;
}
bool AStar::UpdateGs() {

  std::vector<Cell *> open;
  open.reserve(width_ * 2);

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  Cell *q;

  while (not open.empty()) {
    q = PopSmallestH(open);

    for (Cell *k_p : q->nodes)
      if (not k_p->IsDiscovered()) {
        open.push_back(k_p);
        k_p->UpdateG();
        k_p->h = EuclideanDistance(k_p->placement);
        if (k_p->cell_type == CellState::FINISH) return true;
      }
  }

  return false;
}

double AStar::ComputeH(Cell *target) {

  return EuclideanDistance(target->placement);
}

double AStar::EuclideanDistance(const Coord &position) {

  double smallest_distance = 100000000;

  for (auto &f : final_points_) {
    double distance = pow(position.x - f.x, 2) + pow(position.y - f.y, 2);
    if (distance < smallest_distance) smallest_distance = distance;
  }

  return smallest_distance;
}

double AStar::ManhattanDistance(const Coord &position) {
  double smallest_distance = 100000000;

  for (auto &f : final_points_) {
    double distance = Abs(position.x - f.x) + Abs(position.y - f.y);
    if (distance < smallest_distance) smallest_distance = distance;
  }

  return smallest_distance;
}

Cell *AStar::PopSmallestH(std::vector<Cell *> &open_set) {

  int smallest_h = 0;

  for (int i = 1; i < open_set.size(); i++)
    if (open_set[i]->h < open_set[smallest_h]->h)
      smallest_h = i;

  Cell *temp = open_set[smallest_h];
  open_set.erase(open_set.begin() + smallest_h);
  return temp;
}

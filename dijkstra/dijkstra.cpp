//
// Created by piotr on 05/10/2021.
//

#include "dijkstra.h"
#include <algorithm>

Dijkstra::Dijkstra(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++)
      ConnectNeighbours({x, y});
}

Dijkstra::Dijkstra(const Dijkstra &other) : width_(other.width_), height_(other.height_) {
  copy_plane_.reserve(width_ * height_);

  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);

  for (const auto &f : other.starting_points_)
    starting_points_ = other.starting_points_;

  for (const auto &f : other.final_points_)
    final_points_ = other.final_points_;

  for (const auto &f : other.shortest_path_)
    shortest_path_ = other.shortest_path_;
}
Dijkstra &Dijkstra::operator=(const Dijkstra &other) {

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

void Dijkstra::ConnectNeighbours(const Coord &position) {
  if (GetCell(position).cell_type == CellState::WALL) return;
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_)
      if (GetCell(pn).cell_type != CellState::WALL)
        GetCell(position).Connect(GetCell(pn));
}

std::vector<Coord> Dijkstra::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  UpdateGs(window_handle, color_scheme);

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
std::vector<Coord> Dijkstra::FindPath() {
  ClearGraph();

  UpdateGs();

  GeneratePath();

  return shortest_path_;
}

void Dijkstra::UpdateGs() {
  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
    o->got_g = true;
  }

  std::vector<Cell *> successors;
  while (not open.empty()) {

    Cell *q = PopSmallestH(open);

    for (const auto kP : q->nodes)
      if (not kP->got_g) {
        kP->got_g = true;
        successors.push_back(kP);
        kP->UpdateG();
        if (kP->cell_type == CellState::FINISH) return;
      }

    for (const auto kS : successors) {
      open.push_back(kS);
    }
    successors.clear();
  }
}

void Dijkstra::GeneratePath(Window &window_handle, const ColorScheme &color_scheme) {
  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).got_g) {
      final_point = s;
      break;
    }

  shortest_path_.push_back(final_point);
  while (true) {

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    highlights.HighlightCells(shortest_path_, color_scheme.path);
    window_handle.PushFrame(highlights);

    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;
    Cell *next = GetCell(shortest_path_.back()).GetSmallestG();
    if (not next) {
      shortest_path_.clear();
      return;
    }

    shortest_path_.push_back(next->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}

void Dijkstra::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.Clear();
  }
  shortest_path_.clear();
}
void Dijkstra::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {

  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
    o->got_g = true;
  }
  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  std::vector<Cell *> successors;

  while (not open.empty()) {

    Cell *q = PopSmallestH(open);

    for (const auto kP : q->nodes)
      if (not kP->got_g) {
        kP->got_g = true;
        successors.push_back(kP);
        kP->UpdateG();
        if (kP->cell_type == CellState::FINISH) return;
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
}
void Dijkstra::GeneratePath() {
  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).got_g) {
      final_point = s;
      break;
    }

  shortest_path_.push_back(final_point);
  while (true) {

    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;
    Cell *next = GetCell(shortest_path_.back()).GetSmallestG();
    if (not next) {
      shortest_path_.clear();
      return;
    }

    shortest_path_.push_back(next->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}

double Dijkstra::EuclideanDistance(const Coord &position) {
  double smallest_distance = 100000000;

  for (auto &f : final_points_) {
    double distance = sqrt(pow(position.x - f.x, 2) + pow(position.y - f.y, 2));
    if (distance < smallest_distance) smallest_distance = distance;
  }

  return smallest_distance;
}
Cell *Dijkstra::PopSmallestH(std::vector<Cell *> &open_set) {
  for (const auto &o : open_set)
    o->h = EuclideanDistance(o->placement);

  int smallest_h = 0;

  for (int i = 1; i < open_set.size(); i++)
    if (open_set[i]->h < open_set[smallest_h]->h) smallest_h = i;

  Cell *temp = open_set[smallest_h];
  open_set.erase(open_set.begin() + smallest_h);

  return temp;
}

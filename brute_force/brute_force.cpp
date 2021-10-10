//
// Created by piotr on 05/10/2021.
//

#include "brute_force.h"
#include <algorithm>



BruteForce::BruteForce(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";
}

BruteForce::BruteForce(const BruteForce &other) : width_(other.width_), height_(other.height_) {
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
BruteForce &BruteForce::operator=(const BruteForce &other) {

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

std::vector<Coord> BruteForce::GenNeighbours(const Coord &position) {

  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  std::vector<Coord> neighbours;
  for (const auto &pn : potential_neighbours) {
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      switch (GetCell(pn).cell_type) {
        case CellState::EMPTY:

          if (not GetCell(pn).father_ptr) {
            neighbours.push_back(pn);
            GetCell(neighbours.back()).SetFatherPtr(GetCell(position));

          } else if ((GetCell(position).GetG() + 1) < GetCell(pn).GetG()) {
            GetCell(pn).SetFatherPtr(GetCell(position));
          }

          break;

        case CellState::FINISH:
          GetCell(pn).SetFatherPtr(GetCell(position));
          return std::vector<Coord>({pn});
        default:
          break;
      }
    }
  }
  return neighbours;
}

std::vector<Coord> BruteForce::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  if (not GenerateGraph(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}

void BruteForce::GeneratePath() {
  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).father_ptr) {
      final_point = s;
      break;
    }

  shortest_path_.push_back(final_point);
  while (true) {
    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;
    shortest_path_.push_back(GetCell(shortest_path_.back()).father_ptr->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}

bool BruteForce::GenerateGraph(Window &window_handle, const ColorScheme &color_scheme) {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  open = starting_points_;
  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  while (not open.empty()) {
    std::vector<Coord> successors;

    if (not DYNAMIC_DISPLAY) window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

    for (const auto &q : open) {
      for (const auto &nq : GenNeighbours(q)) {
        successors.push_back(nq);
      }
    }

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    highlights.HighlightCells(successors);
    window_handle.PushFrame(highlights);

    if (SearchBreakingPoint(successors, path_has_been_found)) break;
    open = successors;
  }
  return path_has_been_found;
}
void BruteForce::GeneratePath(Window &window_handle, const ColorScheme &color_scheme) {
  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).father_ptr) {
      final_point = s;
      break;
    }
  shortest_path_.push_back(final_point);
  while (true) {

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);

    highlights.HighlightCells(shortest_path_);
    window_handle.PushFrame(highlights);

    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;
    shortest_path_.push_back(GetCell(shortest_path_.back()).father_ptr->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}
bool BruteForce::SearchBreakingPoint(const std::vector<Coord> &possible_routs, bool &path_has_been_found) {
  path_has_been_found = false;

  // check if this is the end of path

  for (auto &fp : final_points_)
    for (const auto &p : possible_routs)
      if (fp == p) {
        path_has_been_found = true;
        return true;
      }

  return false;
}
std::vector<Coord> BruteForce::FindPath() {
  ClearGraph();

  if (not GenerateGraph()) return {};

  GeneratePath();

  return shortest_path_;
}
bool BruteForce::GenerateGraph() {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  open = starting_points_;
  while (not open.empty()) {
    std::vector<Coord> successors;

    for (const auto &q : open) {
      for (const auto &nq : GenNeighbours(q)) {
        successors.push_back(nq);
      }
    }
    if (SearchBreakingPoint(successors, path_has_been_found)) break;
    open = successors;
  }
  return path_has_been_found;
}
void BruteForce::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.father_ptr = nullptr;
  }

  shortest_path_.clear();
}

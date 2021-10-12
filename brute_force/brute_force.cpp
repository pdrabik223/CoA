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

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++)
      ConnectNeighbours({x, y});
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

void BruteForce::ConnectNeighbours(const Coord &position) {
  if (GetCell(position).cell_type == CellState::WALL) return;
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);
  for (const auto &pn : potential_neighbours) {
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      if (GetCell(pn).cell_type != CellState::WALL) {
        GetCell(position).Connect(GetCell(pn));
      }
    }
  }
}
void BruteForce::GenNeighbours(const Coord &position, std::vector<Coord> &target_vec) {

  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  for (const auto &pn : potential_neighbours) {
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {

      switch (GetCell(pn).cell_type) {
        case CellState::EMPTY:
          GetCell(position).Connect(GetCell(pn));

          if (not GetCell(pn).is_connected) {
            target_vec.push_back(pn);
            GetCell(pn).is_connected = true;
          }
          break;

        case CellState::FINISH:
          GetCell(position).Connect(GetCell(pn));

          GetCell(pn).Connect(GetCell(position));
          GetCell(pn).is_connected = true;

          target_vec.clear();
          target_vec.push_back(pn);
          return;
        case CellState::START:
          GetCell(position).Connect(GetCell(pn));
          break;

        default:
          break;
      }
    }
  }
}

std::vector<Coord> BruteForce::FindPath(Window &window_handle, const ColorScheme &color_scheme) {

  ClearGraph();

  //  if (not GenerateGraph(window_handle, color_scheme)) return {};

  UpdateGs(window_handle, color_scheme);

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
std::vector<Coord> BruteForce::FindPath() {
  ClearGraph();

  //  if (not GenerateGraph()) return {};

  UpdateGs();

  GeneratePath();

  return shortest_path_;
}
void BruteForce::GeneratePath() {

  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).is_connected) {
      final_point = s;
      break;
    }

  shortest_path_.push_back(final_point);
  while (true) {
    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;
    if (GetCell(shortest_path_.back()).GetSmallestG() == nullptr) {
      shortest_path_.clear();
      return;
    }
    shortest_path_.push_back(GetCell(shortest_path_.back()).GetSmallestG()->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}

void BruteForce::UpdateGs() {
  std::vector<Cell *> open;

  for (const auto &s : starting_points_)
    open.push_back(&GetCell(s));

  for (auto o : open) {
    o->g = 0;
    o->got_g = true;
  }

  std::vector<Cell *> successors;
  while (not open.empty()) {

    for (const auto &q : open)
      for (const auto kP : q->nodes)
        if (not kP->got_g) {
          kP->got_g = true;
          successors.push_back(kP);
          // if (kP->g > q->g + 1) {
          kP->g = q->g + 1;
          // }
        }

    open = successors;
    successors.clear();
  }
}
bool BruteForce::GenerateGraph(Window &window_handle, const ColorScheme &color_scheme) {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  open = starting_points_;
  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  while (not open.empty()) {
    std::vector<Coord> successors;

    if (not DYNAMIC_DISPLAY) window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

    for (const auto &q : open)
      GenNeighbours(q, successors);

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

bool BruteForce::GenerateGraph() {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  open = starting_points_;
  while (not open.empty()) {
    std::vector<Coord> successors;

    for (const auto &q : open)
      GenNeighbours(q, successors);

    if (SearchBreakingPoint(successors, path_has_been_found)) break;
    open = successors;
  }
  return path_has_been_found;
}
void BruteForce::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.Clear();
  }
  shortest_path_.clear();
}
void BruteForce::UpdateGs(Window &window_handle, const ColorScheme &color_scheme) {
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

    for (const auto &q : open)
      for (const auto kP : q->nodes) {

        if (not kP->got_g) {
          kP->got_g = true;
          successors.push_back(kP);
          // if (kP->g > q->g + 1) {
          kP->g = q->g + 1;
          // }
          if (kP->cell_type == CellState::FINISH) return;
        }
      }
    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    std::vector<Coord> highlighted_positions;

    highlighted_positions.reserve(successors.size());
    for (const auto kS : successors) {
      highlighted_positions.push_back(kS->placement);
    }
    highlights.HighlightCells(highlighted_positions);
    window_handle.PushFrame(highlights);

    open = successors;
    successors.clear();
  }
}

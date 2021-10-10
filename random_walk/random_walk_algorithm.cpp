//
// Created by piotr on 05/10/2021.
//

#include "random_walk_algorithm.h"

RandomWalkAlgorithm::RandomWalkAlgorithm(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";
}
RandomWalkAlgorithm::RandomWalkAlgorithm(const RandomWalkAlgorithm &other) : width_(other.width_), height_(other.height_) {
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
RandomWalkAlgorithm &RandomWalkAlgorithm::operator=(const RandomWalkAlgorithm &other) {
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

std::vector<Coord> RandomWalkAlgorithm::GenNeighbours(const Coord &position) {

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
            GetCell(position).SetSonPtr(GetCell(neighbours.back()));
          } else if (GetCell(position).GetG() < GetCell(pn).GetG()) {
            GetCell(pn).SetFatherPtr(GetCell(position));
          }
          //          else if (GetCell(position).GetG() > GetCell(pn).GetG() ) {
//            GetCell(position).SetFatherPtr(GetCell(pn));
//          }

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
void RandomWalkAlgorithm::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.father_ptr = nullptr;
    g.son_ptr = nullptr;
  }
  shortest_path_.clear();
}
bool RandomWalkAlgorithm::SearchBreakingPoint(const std::vector<Coord> &possible_routs, bool &path_has_been_found) {
  path_has_been_found = false;

  // check if this is the end of path
  if (possible_routs.size() == 1) {
    for (auto &p : final_points_)
      if (possible_routs.front() == p) {
        path_has_been_found = true;
        return true;
      }
  }
  return false;
}
std::vector<Coord> RandomWalkAlgorithm::FindPath(Window &window_handle, const ColorScheme &color_scheme) {
  ClearGraph();

  // convert cell grid to proper graph
  // if connection between any start and any finish won't be found GenerateGraph will return false, and we end the FindPath function
  if (not GenerateGraph(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
void RandomWalkAlgorithm::GeneratePath() {
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
void RandomWalkAlgorithm::GeneratePath(Window &window_handle, const ColorScheme &color_scheme) {
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
Coord RandomWalkAlgorithm::PopRandomCell(std::vector<Coord> &positions) {

  unsigned rand_pos = rand() % positions.size();
  auto temp = positions[rand_pos];
  positions.erase(positions.begin() + rand_pos);

  return temp;
}
bool RandomWalkAlgorithm::GenerateGraph() {
  bool path_has_been_found = false;

  std::vector<Coord> open;

  open = starting_points_;
  Coord q;
  std::vector<Coord> successors;

  while (not open.empty()) {
    q = PopRandomCell(open);

    successors = GenNeighbours(q);

    if (SearchBreakingPoint(successors, path_has_been_found)) break;

    for (const auto &s : successors)
      open.push_back(s);
  }

  return path_has_been_found;
}
std::vector<Coord> RandomWalkAlgorithm::FindPath() {
  ClearGraph();

  // convert cell grid to proper graph
  // if connection between any start and any finish won't be found GenerateGraph will return false, and we end the FindPath function
  if (not GenerateGraph()) return {};

  GeneratePath();

  return shortest_path_;
}
bool RandomWalkAlgorithm::GenerateGraph(Window &window_handle, const ColorScheme &color_scheme) {
  bool path_has_been_found = false;

  std::vector<Coord> open;

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  open = starting_points_;

  Coord q;
  std::vector<Coord> successors;

  while (not open.empty()) {
    q = PopRandomCell(open);
    if (not DYNAMIC_DISPLAY) window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

    successors = GenNeighbours(q);

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    highlights.HighlightCells(successors);
    window_handle.PushFrame(highlights);

    if (SearchBreakingPoint(successors, path_has_been_found)) break;

    for (const auto &s : successors)
      open.push_back(s);
  }

  return path_has_been_found;
}

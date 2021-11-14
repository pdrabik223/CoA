//
// Created by piotr on 20/10/2021.
//
#include "graph_base.h"

void GraphBase::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.Clear();
  }
  shortest_path_.clear();
}
void GraphBase::ConnectNeighboursVonNeuman(const Coord &position) {
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

void GraphBase::ConnectNeighboursMoore(const Coord &position) {
  if (GetCell(position).cell_type == CellState::WALL) return;
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  potential_neighbours.emplace_back(position.x - 1, position.y - 1);
  potential_neighbours.emplace_back(position.x + 1, position.y - 1);
  potential_neighbours.emplace_back(position.x - 1, position.y + 1);
  potential_neighbours.emplace_back(position.x + 1, position.y + 1);

  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_)
      if (GetCell(pn).cell_type != CellState::WALL)
        GetCell(position).Connect(GetCell(pn));
}
void GraphBase::GeneratePath() {

  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).IsDiscovered()) {
      final_point = s;
      break;
    }

  shortest_path_.push_back(final_point);
  while (true) {

    if (GetCell(shortest_path_.back()).cell_type == CellState::START) break;

    Cell *next;

    next = GetCell(shortest_path_.back()).GetSmallestG();

    if (not next) {
      shortest_path_.clear();
      return;
    }

    shortest_path_.push_back(next->placement);
  }
  std::reverse(shortest_path_.begin(), shortest_path_.end());
}
void GraphBase::GeneratePath(Window &window_handle, const ColorScheme &color_scheme) {
  Coord final_point;
  for (const auto &s : final_points_)
    if (GetCell(s).IsDiscovered()) {
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
GraphBase::GraphBase(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";

  if (neighbourhood_ == Neighbourhood::VON_NEUMAN)
    for (int y = 0; y < other.GetHeight(); y++)
      for (int x = 0; x < other.GetWidth(); x++)
        ConnectNeighboursVonNeuman({x, y});
  else
    for (int y = 0; y < other.GetHeight(); y++)
      for (int x = 0; x < other.GetWidth(); x++)
        ConnectNeighboursMoore({x, y});
}

GraphBase::GraphBase(const Plane &other, Neighbourhood neighbourhood) : width_(other.GetWidth()), height_(other.GetHeight()), neighbourhood_(neighbourhood) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";

  if (neighbourhood_ == Neighbourhood::VON_NEUMAN)
    for (int y = 0; y < other.GetHeight(); y++)
      for (int x = 0; x < other.GetWidth(); x++)
        ConnectNeighboursVonNeuman({x, y});
  else
    for (int y = 0; y < other.GetHeight(); y++)
      for (int x = 0; x < other.GetWidth(); x++)
        ConnectNeighboursMoore({x, y});
}

GraphBase::GraphBase(const GraphBase &other) {
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
void GraphBase::HighlightPositions(Window &window_handle, const ColorScheme &color_scheme, const std::vector<Cell *> &successors) {
  WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
  std::vector<Coord> highlighted_positions;

  highlighted_positions.reserve(successors.size());
  for (const auto kS : successors) {
    highlighted_positions.push_back(kS->placement);
  }
  highlights.HighlightCells(highlighted_positions);
  window_handle.PushFrame(highlights);
}

double GraphBase::EuclideanDistance(const Coord &position) {
  if (final_points_.size() == 1)
    return pow(position.x - final_points_.back().x, 2) + pow(position.y - final_points_.back().y, 2);

  double smallest_distance = 100000000;

  for (auto &f : final_points_) {
    double distance = pow(position.x - f.x, 2) + pow(position.y - f.y, 2);
    if (distance < smallest_distance) smallest_distance = distance;
  }

  return smallest_distance;
}
double GraphBase::ManhattanDistance(const Coord &position) {
  if (final_points_.size() == 1)
    return Abs(position.x - final_points_.back().x) + Abs(position.y - final_points_.back().y);

  double smallest_distance = 100000000;

  for (auto &f : final_points_) {
    double distance = Abs(position.x - f.x) + Abs(position.y - f.y);
    if (distance < smallest_distance) smallest_distance = distance;
  }

  return smallest_distance;
}
double GraphBase::PDistance(const Coord &position) {
  if (final_points_.size() == 1) {

    unsigned d_x = Abs(position.x - final_points_.back().x);
    unsigned d_y = Abs(position.y - final_points_.back().y);

    return d_x > d_y ? d_x : d_y;
  }

  unsigned smallest_distance = 100000000;

  for (auto &f : final_points_) {
    unsigned d_x = Abs(position.x - f.x);
    unsigned d_y = Abs(position.y - f.y);

    unsigned distance = d_x > d_y ? d_x : d_y;
    if (distance < smallest_distance)
      smallest_distance = distance;
  }

  return smallest_distance;
}

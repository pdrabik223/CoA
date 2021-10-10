#include "dijkstra.h"
#include <iostream>
#define INT(x) x.ToInt(width_)

dijkstra::Dijkstra::Dijkstra(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.push_back({other.GetCell({x, y}), {x, y}});
      if (other.GetCell({x, y}) == CellState::START) starting_points_.emplace_back(x, y);
      if (other.GetCell({x, y}) == CellState::FINISH) final_points_.emplace_back(x, y);
    }

  if (starting_points_.empty() or final_points_.empty()) throw "bad plane error";
}
dijkstra::Dijkstra::Dijkstra(const dijkstra::Dijkstra &other) : width_(other.width_), height_(other.height_) {
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
dijkstra::Dijkstra &dijkstra::Dijkstra::operator=(const dijkstra::Dijkstra &other) {
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
std::vector<Coord> dijkstra::Dijkstra::GenNeighbours(const Coord &position) {

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

          } else if (GetCell(position).GetG()  < GetCell(pn).GetG()) {
            GetCell(pn).SetFatherPtr(GetCell(position));
          }
          else if (GetCell(position).GetG() > GetCell(pn).GetG() ) {
            GetCell(position).SetFatherPtr(GetCell(pn));
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
void dijkstra::Dijkstra::ApplyHDistance(std::vector<Coord> &cells) {
  for (auto &c : cells) {
    GetCell(c).SetH(EuclideanDistance(c));
  }
}

double dijkstra::Dijkstra::EuclideanDistance(const Coord &position) {

  double smallest_distance = UINT32_MAX;
  double distance_to_f = 0;

  for (const auto &f : final_points_) {

    distance_to_f = sqrt(pow((position.x - f.x), 2) + pow((position.y - f.y), 2));

    if (distance_to_f < smallest_distance)
      smallest_distance = distance_to_f;
  }

  return smallest_distance;
}

Coord dijkstra::Dijkstra::PopBestFCell(std::vector<Coord> &positions) {

  int smallest_f_id = 0;

  for (int p = 1; p < positions.size(); ++p)
    if (GetCell(positions[p]).GetF() < GetCell(positions[smallest_f_id]).GetF())
      smallest_f_id = p;

  auto temp = positions[smallest_f_id];
  positions.erase(positions.begin() + smallest_f_id);

  return temp;
}

std::vector<Coord> dijkstra::Dijkstra::FindPath() {

  ClearGraph();

  // convert cell grid to proper graph
  // if connection between any start and any finish won't be found GenerateGraph will return false, and we end the FindPath function
  if (not GenerateGraph()) return {};

  GeneratePath();

  return shortest_path_;
}

bool dijkstra::Dijkstra::GenerateGraph() {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  std::vector<Coord> closed;

  open = starting_points_;

  Coord q;
  std::vector<Coord> successors;

  while (not open.empty()) {
    q = PopBestFCell(open);

    successors = GenNeighbours(q);
    if (SearchBreakingPoint(successors, path_has_been_found)) break;

    ApplyHDistance(successors);

    for (const auto &s : successors)
      open.push_back(s);
  }

  return path_has_been_found;
}
void dijkstra::Dijkstra::GeneratePath() {

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

bool dijkstra::Dijkstra::SearchBreakingPoint(const std::vector<Coord> &possible_routs, bool &path_has_been_found) {
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

std::vector<Coord> dijkstra::Dijkstra::FindPath(Window &window_handle, const ColorScheme &color_scheme) {
  ClearGraph();

  if (not GenerateGraph(window_handle, color_scheme)) return {};

  GeneratePath(window_handle, color_scheme);

  return shortest_path_;
}
void dijkstra::Dijkstra::ClearGraph() {
  for (auto &g : copy_plane_) {
    g.father_ptr = nullptr;
    g.son_ptr = nullptr;
  }

  shortest_path_.clear();
}
dijkstra::Dijkstra::~Dijkstra() {
  ClearGraph();
}

bool dijkstra::Dijkstra::GenerateGraph(Window &window_handle, const ColorScheme &color_scheme) {
  bool path_has_been_found = false;

  std::vector<Coord> open;
  std::vector<Coord> closed;

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  open = starting_points_;
  ApplyHDistance(open);

  Coord q;
  std::vector<Coord> successors;

  while (not open.empty()) {
    q = PopBestFCell(open);
    if (not DYNAMIC_DISPLAY) window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

    successors = GenNeighbours(q);

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    highlights.HighlightCells(successors);
    window_handle.PushFrame(highlights);

    if (SearchBreakingPoint(successors, path_has_been_found)) break;

    ApplyHDistance(successors);

    for (const auto &s : successors)
      open.push_back(s);
  }

  return path_has_been_found;
}

void dijkstra::Dijkstra::GeneratePath(Window &window_handle, const ColorScheme &color_scheme) {
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

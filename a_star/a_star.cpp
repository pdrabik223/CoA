//
// Created by piotr on 06/10/2021.
//

#include "a_star.h"

a_star::AStar::AStar(const a_star::AStar &other) {

}
unsigned a_star::AStar::ManhattanDistance(const Coord &position, const std::vector<Coord> &finish_points) {
  std::vector<unsigned> distances(finish_points.size());

  unsigned lowest_distance = CELL_MAX;
  unsigned distance;
  for (const auto &p : finish_points) {
    distance = abs(position.x - p.x) + abs(position.y - p.y);
    if(distance < lowest_distance) lowest_distance = distance;
  }
  return lowest_distance;

}
a_star::AStar::AStar(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  bool is_there_starting_point = false;
  bool is_there_destination_point = false;

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++) {
      copy_plane_.emplace_back(other.GetCell({x, y}));
      if (other.GetCell({x, y}) == CellState::START) is_there_starting_point = true;
      if (other.GetCell({x, y}) == CellState::FINISH) is_there_destination_point = true;
    }

  if (not is_there_starting_point or not is_there_destination_point) throw "bad plane error";
}
a_star::AStar &a_star::AStar::operator=(const a_star::AStar &other) {
  if (&other == this) return *this;

  width_ = other.width_;
  height_ = other.height_;
  copy_plane_.reserve(width_ * height_);

  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);

  return *this;
}
std::vector<Coord> a_star::AStar::FindPath() {
  std::vector<Coord> open;
  std::vector<Coord> closed;

  std::vector<Coord> starting_points;
  std::vector<Coord> final_points;
  GetStartAndFinish(starting_points, final_points);

  unsigned iteration = 0;

  bool path_has_been_found = false;

  open = GenNeighboursForAllPositions(starting_points);
  ApplyDistance(open,final_points);

  Coord q;
    while (!open.empty()) {
      q = GetBestGCell(open);
      open.erase(std::find(open.begin(), open.end(),q));
      



    ApplyIteration(open, starting_points);
    open = GenNeighboursForAllPositions(closed);

    }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    open.push_back(closed.front());

    closed = GenNeighboursButIgnoreDistance(closed.front());
    while (1 < 2) {

      open.emplace_back(GetBestCell(closed));

      for (auto &p : starting_points) {
        if (open.back() == p) {
          std::reverse(open.begin(), open.end());
          return open;
        } else
          closed = GenNeighboursButIgnoreDistance(open.back());
      }
    }
  }

  // else return empty path object
  return open;

}
std::vector<Coord> a_star::AStar::GenNeighbours(const Coord &position) {
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  std::vector<Coord> neighbours;
  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      switch (copy_plane_[pn.ToInt(width_)].cell_type) {
        case CellState::EMPTY:
          if (copy_plane_[pn.ToInt(width_)].distance == CELL_MAX) {
            neighbours.push_back(pn);
          }
          break;
        case CellState::FINISH:
          return std::vector<Coord>({pn});
        default:
          break;
      }
    }

  return neighbours;
}
std::vector<Coord> a_star::AStar::GenNeighboursForAllPositions(const std::vector<Coord> &positions) {
  std::vector<Coord> solution;
  for (auto &c : positions) {
    for (auto &gc : GenNeighbours(c))
      solution.push_back(gc);
  }

  std::sort(solution.begin(), solution.end());

  auto last = std::unique(solution.begin(), solution.end());

  solution.erase(last, solution.end());
  return solution;
}
std::vector<Coord> a_star::AStar::GenNeighboursButIgnoreDistance(const Coord &position) {
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  std::vector<Coord> neighbours;
  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      switch (copy_plane_[pn.ToInt(width_)].cell_type) {
        case CellState::EMPTY:
        case CellState::START:
          neighbours.push_back(pn);

        default:
          break;
      }
    }

  return neighbours;
}
void a_star::AStar::ApplyIteration(std::vector<Coord> &cells, const std::vector<Coord>& start_points) {
  for (auto &c : cells)
    copy_plane_[c.ToInt(width_)].distance = ManhattanDistance(c,start_points);
}
Coord a_star::AStar::GetBestCell(std::vector<Coord> &positions) {
  Coord minimal_cell_position;
  Cell minimal_cell;

  std::reverse(positions.begin(), positions.end());

  int i = 0;
  while (1 < 2) {
    minimal_cell_position = positions[i];
    minimal_cell = copy_plane_[minimal_cell_position.ToInt(width_)];

    if (minimal_cell.distance != CELL_MAX) break;
    i++;
  }

  for (; i < positions.size(); i++)

    if (copy_plane_[positions[i].ToInt(width_)].distance == CELL_MAX) continue;
    else if (copy_plane_[positions[i].ToInt(width_)].distance < minimal_cell.distance) {
      minimal_cell_position = positions[i];
      minimal_cell = copy_plane_[minimal_cell_position.ToInt(width_)];
    }

  return minimal_cell_position;
}

void a_star::AStar::GetStartAndFinish(std::vector<Coord> &start_points, std::vector<Coord> &finish_points) {
  for (int i = 0; i < width_ * height_; i++)
    if (copy_plane_[i].cell_type == CellState::START) {

      start_points.emplace_back((int) (i % width_), (int) (i / width_));

    } else if (copy_plane_[i].cell_type == CellState::FINISH) {
      finish_points.emplace_back((int) (i % width_), (int) (i / width_));
    }

}

void a_star::AStar::ApplyDistance(std::vector<Coord> &cells, const std::vector<Coord> &finish_points) {
  for(auto& c:cells)
    copy_plane_[c.ToInt(width_)].SetG(ManhattanDistance(c,finish_points));

}
Coord a_star::AStar::GetBestGCell(std::vector<Coord> &positions) {
  Coord lowest_g = positions.front();

  for(const auto &p:positions)
    if(copy_plane_[p.ToInt(width_)].GetG() < copy_plane_[lowest_g.ToInt(width_)].GetG()) lowest_g = p;


  return lowest_g;

}

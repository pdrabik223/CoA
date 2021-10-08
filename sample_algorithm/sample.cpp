//
// Created by piotr on 05/10/2021.
//

#include "sample.h"
#include <algorithm>
#define FUCK false
#include <algorithm>
#include <assert.h>

Sample::Sample(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
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

Sample::Sample(const Sample &other) : width_(other.width_), height_(other.height_) {
  copy_plane_.reserve(width_ * height_);
  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);
}
Sample &Sample::operator=(const Sample &other) {

  if (&other == this) return *this;

  width_ = other.width_;
  height_ = other.height_;
  copy_plane_.reserve(width_ * height_);

  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);

  return *this;
}
void Sample::GetStartAndFinish(std::vector<Coord> &start_points, std::vector<Coord> &finish_points) {
  for (int i = 0; i < width_ * height_; i++)
    if (copy_plane_[i].cell_type == CellState::START) {

      start_points.emplace_back((int) (i % width_), (int) (i / width_));

    } else if (copy_plane_[i].cell_type == CellState::FINISH) {
      finish_points.emplace_back((int) (i % width_), (int) (i / width_));
    }
}
bool SearchBreakingPoint(const std::vector<Coord> &possible_routs, const std::vector<Coord> &final_points, bool &path_has_been_found) {
  path_has_been_found = false;
  if (possible_routs.size() > 1) return false;

  // check if this is the end of path
  if (possible_routs.size() == 1) {
    for (auto &p : final_points)
      if (possible_routs.front() == p) {
        path_has_been_found = true;
        return true;
      }

    // check if there even is a path to final point
  } else {
    if (possible_routs.empty()) {
      path_has_been_found = false;
      return true;
    };
  }
  return false;
}

std::vector<Coord> Sample::FindPath() {
  std::vector<Coord> solution;
  std::vector<Coord> currently_analyzed_cells_buffer;

  std::vector<Coord> starting_points;
  std::vector<Coord> final_points;
  GetStartAndFinish(starting_points, final_points);

  // both of points will get populated that's Winger guarantee
  // and also constructor would scream if plane object was incorrect

  unsigned iteration = 0;
  // gen first batch of cells
  currently_analyzed_cells_buffer = GenNeighboursForAllPositions(starting_points);

  bool path_has_been_found = false;

  while (1 < 2) {

    if (SearchBreakingPoint(currently_analyzed_cells_buffer, final_points, path_has_been_found)) break;

    // if not apply weights witch basically represent distance from origin point
    ApplyIteration(currently_analyzed_cells_buffer, ++iteration);

    // gen new bach of points, based on previously visited
    currently_analyzed_cells_buffer = GenNeighboursForAllPositions(currently_analyzed_cells_buffer);
  }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    solution.push_back(currently_analyzed_cells_buffer.front());

    currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(currently_analyzed_cells_buffer.front());
    while (1 < 2) {

      solution.emplace_back(GetBestCell(currently_analyzed_cells_buffer));

      for (auto &p : starting_points) {
        if (solution.back() == p) {
          std::reverse(solution.begin(), solution.end());
          return solution;
        } else
          currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(solution.back());
      }
    }
  }

  // else return empty path object
  return solution;
}

std::vector<Coord> Sample::GenNeighbours(const Coord &position) {

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

void Sample::ApplyIteration(std::vector<Coord> &cells, unsigned int iteration) {
  for (auto &c : cells)
    copy_plane_[c.ToInt(width_)].distance = iteration;
}

std::vector<Coord> Sample::GenNeighboursForAllPositions(const std::vector<Coord> &positions) {
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

Coord Sample::GetBestCell(std::vector<Coord> &positions) {
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
std::vector<Coord> Sample::FindPath(Window &window_handle, const ColorScheme &color_scheme) {
  std::vector<Coord> solution;
  std::vector<Coord> currently_analyzed_cells_buffer;

  std::vector<Coord> starting_points;
  std::vector<Coord> final_points;
  GetStartAndFinish(starting_points, final_points);

  // both of points will get populated that's Winger guarantee
  // and also constructor would scream if plane object was incorrect

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

  unsigned iteration = 0;
  // gen first batch of cells
  currently_analyzed_cells_buffer = GenNeighboursForAllPositions(starting_points);

  bool path_has_been_found = false;

  while (1 < 2) {

    // check Neighbours
    //   CheckNeighbours(currently_analyzed_cells_buffer);

    if (SearchBreakingPoint(currently_analyzed_cells_buffer, final_points, path_has_been_found)) break;

    // if not apply weights witch basically represent distance from origin point
    ApplyIteration(currently_analyzed_cells_buffer, ++iteration);

    window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_, color_scheme));

    // gen new bach of points, based on previously visited

    currently_analyzed_cells_buffer = GenNeighboursForAllPositions(currently_analyzed_cells_buffer);

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);

    printf("(1) iteration: %d, size: %u\n ", iteration, currently_analyzed_cells_buffer.size());

    highlights.HighlightCells(currently_analyzed_cells_buffer);
    window_handle.PushFrame(highlights);
  }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    solution.push_back(currently_analyzed_cells_buffer.front());

    WindowPlane highlights(copy_plane_, width_, height_, color_scheme);
    highlights.HighlightCells(solution);
    window_handle.PushFrame(highlights);

    currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(currently_analyzed_cells_buffer.front());
    while (1 < 2) {

      solution.emplace_back(GetBestCell(currently_analyzed_cells_buffer));
      highlights.HighlightCells(solution);
      window_handle.PushFrame(highlights);

      for (auto &p : starting_points) {
        if (solution.back() == p) {
          std::reverse(solution.begin(), solution.end());
          return solution;
        } else
          currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(solution.back());
      }
    }
  }

  // else return empty path object
  return solution;
}

std::vector<Coord> Sample::GenNeighboursButIgnoreDistance(const Coord &position) {
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

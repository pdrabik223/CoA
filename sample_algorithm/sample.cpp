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
      if (other.GetCell({x, y}) == CellType::START) is_there_starting_point = true;
      if (other.GetCell({x, y}) == CellType::FINISH) is_there_destination_point = true;
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
void Sample::GetStartAndFinish(Coord &start_point, Coord &finish_point) {
  for (int i = 0; i < width_ * height_; i++)
    if (copy_plane_[i].cell_type == CellType::START) {

      start_point = {(int) (i % width_), (int) (i / width_)};

    } else if (copy_plane_[i].cell_type == CellType::FINISH) {
      finish_point = {(int) (i % width_), (int) (i / width_)};
    }
}
bool SearchBreakingPoint(const std::vector<Coord> &possible_routs, const Coord &final_point, bool &path_has_been_found) {
  path_has_been_found = false;
  if (possible_routs.size() > 1) return false;

  // check if this is the end of path
  if (possible_routs.size() == 1) {
    if (possible_routs.front() == final_point) {
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

  Coord starting_point;
  Coord final_point;
  GetStartAndFinish(starting_point, final_point);

  // both of points will get populated that's Winger guarantee
  // and also constructor would scream if plane object was incorrect

  unsigned iteration = 0;
  // gen first batch of cells
  currently_analyzed_cells_buffer = GenNeighbours(starting_point);

  bool path_has_been_found = false;

  while (1 < 2) {

    // check Neighbours
    //   CheckNeighbours(currently_analyzed_cells_buffer);

    if (SearchBreakingPoint(currently_analyzed_cells_buffer, final_point, path_has_been_found)) break;



    // if not apply weights witch basically represent distance from origin point
    ApplyIteration(currently_analyzed_cells_buffer, ++iteration);


    // gen new bach of points, based on previously visited
    currently_analyzed_cells_buffer = GenNeighbours(currently_analyzed_cells_buffer);

  }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    solution.emplace_back(final_point);

    currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(final_point);
    while (1 < 2) {

      solution.emplace_back(GetBestCell(currently_analyzed_cells_buffer));

      if (solution.back() == starting_point)
        break;
      else
        currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(solution.back());
    }
    std::reverse(solution.begin(), solution.end());
  }

  // else return empty path object
  return solution;
}

std::vector<Coord> Sample::GenNeighbours(const Coord &position) {
  // TODO test neighbours generation function

  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  std::vector<Coord> neighbours;
  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      switch (copy_plane_[pn.ToInt(width_)].cell_type) {
        case CellType::EMPTY:
          if (copy_plane_[pn.ToInt(width_)].distance == CELL_MAX) {
            neighbours.push_back(pn);
          }
          break;
        case CellType::FINISH:
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

std::vector<Coord> Sample::GenNeighbours(const std::vector<Coord> &positions) {
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
std::vector<Coord> Sample::FindPath(Window &window_handle) {
  std::vector<Coord> solution;
  std::vector<Coord> currently_analyzed_cells_buffer;

  Coord starting_point;
  Coord final_point;
  GetStartAndFinish(starting_point, final_point);

  // both of points will get populated that's Winger guarantee
  // and also constructor would scream if plane object was incorrect

  window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_));

  unsigned iteration = 0;
  // gen first batch of cells
  currently_analyzed_cells_buffer = GenNeighbours(starting_point);

  bool path_has_been_found = false;

  while (1 < 2) {

    // check Neighbours
    //   CheckNeighbours(currently_analyzed_cells_buffer);

    if (SearchBreakingPoint(currently_analyzed_cells_buffer, final_point, path_has_been_found)) break;



    // if not apply weights witch basically represent distance from origin point
    ApplyIteration(currently_analyzed_cells_buffer, ++iteration);

    window_handle.PushFrame(WindowPlane(copy_plane_, width_, height_));

    // gen new bach of points, based on previously visited

    currently_analyzed_cells_buffer = GenNeighbours(currently_analyzed_cells_buffer);

    WindowPlane highlights(copy_plane_, width_, height_);

    printf("(1) iteration: %d, size: %u\n ", iteration, currently_analyzed_cells_buffer.size());

    highlights.HighlightCells(currently_analyzed_cells_buffer);
    window_handle.PushFrame(highlights);
  }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    solution.emplace_back(final_point);

    WindowPlane highlights(copy_plane_, width_, height_);
    highlights.HighlightCells(solution);
    window_handle.PushFrame(highlights);

    currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(final_point);
    while (1 < 2) {

      solution.emplace_back(GetBestCell(currently_analyzed_cells_buffer));
      highlights.HighlightCells(solution);
      window_handle.PushFrame(highlights);

      if (solution.back() == starting_point)
        break;
      else
        currently_analyzed_cells_buffer = GenNeighboursButIgnoreDistance(solution.back());
    }
    std::reverse(solution.begin(), solution.end());
  }

  // else return empty path object
  return solution;
}
std::vector<Coord> Sample::GenNeighboursButIgnoreDistance(const Coord& position) {
  std::vector<Coord> potential_neighbours;

  potential_neighbours.emplace_back(position.x, position.y - 1);
  potential_neighbours.emplace_back(position.x, position.y + 1);
  potential_neighbours.emplace_back(position.x - 1, position.y);
  potential_neighbours.emplace_back(position.x + 1, position.y);

  std::vector<Coord> neighbours;
  for (const auto &pn : potential_neighbours)
    if (pn.x >= 0 and pn.y >= 0 and pn.y < height_ and pn.x < width_) {
      switch (copy_plane_[pn.ToInt(width_)].cell_type) {
        case CellType::EMPTY:
        case CellType::START:
          neighbours.push_back(pn);


        default:
          break;
      }
    }

  return neighbours;
}

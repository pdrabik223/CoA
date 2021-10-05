//
// Created by piotr on 05/10/2021.
//

#include "sample.h"
#include <algorithm>
#define FUCK false
#include <assert.h>
Sample::Sample(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
//  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

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

std::vector<Coord> Sample::FindPath() {
  std::vector<Coord> solution;
  std::vector<Coord> next_step_cells_buffer;
  std::vector<Coord> currently_analyzed_cells_buffer;

  Coord starting_point;
  Coord final_point;

  for (int i = 0; i < width_ * height_; i++)
    if (copy_plane_[i].cell_type == CellType::START) {
      starting_point = {(int) (i / width_), (int) (i % width_)};
      break;
    } else if (copy_plane_[i].cell_type == CellType::FINISH) {
      final_point = {(int) (i / width_), (int) (i % width_)};
      break;
    }
  // both of points will get populated that's Winger guarantee
  // and also constructor would scream if plane object was incorrect

  unsigned iteration = 0;
  // gen first batch of cells
  currently_analyzed_cells_buffer = GenNeighbours(starting_point);

  bool path_has_been_found = false;

  while (1 < 2) {

    // check Neighbours
    CheckNeighbours(currently_analyzed_cells_buffer);

    // check if this is the end of path
    if (currently_analyzed_cells_buffer.size() == 1)
      if (currently_analyzed_cells_buffer.front() == final_point) {
        path_has_been_found = true;
        break;
      }


    // check if there even is a path to final point
    if (currently_analyzed_cells_buffer.empty()) {
      path_has_been_found = false;
      break;
    }
    // if not apply weights witch basically represent distance from origin point
    ApplyIteration(currently_analyzed_cells_buffer, ++iteration);

    // gen new bach of points, based on previously visited
    currently_analyzed_cells_buffer = GenNeighbours(currently_analyzed_cells_buffer);
  }

  // if path has been found than work your way backwards from final point to starting point, but only visit cells with the lowest distance, so the path generated is optimal
  if (path_has_been_found) {

    currently_analyzed_cells_buffer = GenNeighbours(final_point);
    while (1 < 2) {
      solution.emplace_back(GetBestCell(currently_analyzed_cells_buffer));
      if (solution.back() == starting_point) break;
      else currently_analyzed_cells_buffer = GenNeighbours(solution.back());
    }
  }

  // else return empty path object
  return solution;
}

std::vector<Coord> Sample::GenNeighbours(const Coord &position) {
  // TODO test neighbours generation function

  std::vector<Coord> neighbours;
  neighbours.reserve(8);
  neighbours.emplace_back(position.x - 1, position.y - 1);
  neighbours.emplace_back(position.x, position.y - 1);
  neighbours.emplace_back(position.x + 1, position.y - 1);

  neighbours.emplace_back(position.x - 1, position.y);
  neighbours.emplace_back(position.x + 1, position.y);

  neighbours.emplace_back(position.x - 1, position.y + 1);
  neighbours.emplace_back(position.x, position.y + 1);
  neighbours.emplace_back(position.x + 1, position.y + 1);

  for (int i = 7; i >= 0; i--)
    if (neighbours[i].x < 0 or neighbours[i].y < 0 or neighbours[i].y >= height_ or neighbours[i].x >= width_)
      neighbours.erase(neighbours.begin() + i);

  return neighbours;
}

std::vector<Coord> Sample::CheckNeighbours(std::vector<Coord> &neighbours) {
  for (int i = neighbours.size() - 1; i >= 0; i--) {
    if(i<0){
      assert(FUCK);
    }
    if (copy_plane_[neighbours[i].ToInt(width_)].cell_type == CellType::WALL) neighbours.erase(neighbours.begin() + i);

    if (copy_plane_[neighbours[i].ToInt(width_)].cell_type == CellType::START) neighbours.erase(neighbours.begin() + i);
    if (copy_plane_[neighbours[i].ToInt(width_)].cell_type == CellType::FINISH) {
      Coord final_cell = neighbours[i];
      neighbours.clear();
      neighbours.emplace_back(final_cell);
      return neighbours;
    }

    if (copy_plane_[neighbours[i].ToInt(width_)].distance != 0) neighbours.erase(neighbours.begin() + i);
  }
  return neighbours;
}
void Sample::ApplyIteration(std::vector<Coord> &cells, unsigned int iteration) {
  for (auto &c : cells)
    copy_plane_[c.ToInt(width_)].distance = iteration;
}
std::vector<Coord> Sample::GenNeighbours(const std::vector<Coord> &positions) {
  std::vector<Coord> solution;
  for (auto &c : positions)
    for (auto &gc : GenNeighbours(c))
    solution.emplace_back(gc);
  return solution;
}
Coord Sample::GetBestCell(const std::vector<Coord> &positions) {
  Coord minimal_cell_position = positions.front();
  Cell minimal_cell = copy_plane_[minimal_cell_position.ToInt(width_)];
  for(int i =1 ; i<positions.size();i++)
    if(copy_plane_[positions[i].ToInt(width_)] < minimal_cell){
      minimal_cell_position = positions[i];
      minimal_cell = copy_plane_[minimal_cell_position.ToInt(width_)];
    }

  return minimal_cell_position;
}

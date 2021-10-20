//
// Created by piotr on 20/10/2021.
//

#ifndef COA_PATH_SEARCH_GRAPH_BASE_H_
#define COA_PATH_SEARCH_GRAPH_BASE_H_

#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "a_star/cell.h"
#include <vector>

class GraphBase {
 public:
  GraphBase(const Plane &other);
  GraphBase(const GraphBase &other);
  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  virtual std::vector<Coord> FindPath() = 0;

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  virtual std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) = 0;

  virtual ~GraphBase() { ClearGraph(); };

 protected:
  void GeneratePath();
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);
  void ClearGraph();
  void ConnectNeighbours(const Coord &position);
  Cell &GetCell(const Coord &position) { return this->copy_plane_[position.ToInt(width_)]; };

  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;
  std::vector<Coord> starting_points_;
  std::vector<Coord> final_points_;
  std::vector<Coord> shortest_path_;
};

#endif//COA_PATH_SEARCH_GRAPH_BASE_H_

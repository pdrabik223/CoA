//
// Created by piotr on 05/10/2021.
//

#ifndef COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_
#define COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "../sfml_window/window_plane.h"

class RandomWalk {

 public:
  RandomWalk() = delete;
  /// constructs Sample object based of Plane
  RandomWalk(const Plane &other);
  RandomWalk(const RandomWalk &other);
  RandomWalk &operator=(const RandomWalk &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme);

  ~RandomWalk() {
    ClearGraph();
  }

 private:
  void GeneratePath();
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  Cell *PopRandomCell(std::vector<Cell *> &positions);

  Cell &GetCell(const Coord &position) { return copy_plane_[position.ToInt(width_)]; };

  void ClearGraph();

  void ConnectNeighbours(const Coord &position);
  bool UpdateGs();
  bool UpdateGs(Window &window_handle, const ColorScheme &color_scheme);

 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<Cell> copy_plane_;

  std::vector<Coord> starting_points_;
  std::vector<Coord> final_points_;
  std::vector<Coord> shortest_path_;

  };

#endif//COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_

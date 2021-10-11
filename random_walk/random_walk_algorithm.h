//
// Created by piotr on 05/10/2021.
//

#ifndef COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_
#define COA_RANDOM_WALK_RANDOW_WALK_ALGORITHM_H_
#include "../plane/plane.h"
#include "../brute_force/cell.h"
#include "../sfml_window/window.h"
#include "../sfml_window/window_plane.h"

class RandomWalkAlgorithm {

 public:
  RandomWalkAlgorithm() = delete;
  /// constructs Sample object based of Plane
  RandomWalkAlgorithm(const Plane &other);
  RandomWalkAlgorithm(const RandomWalkAlgorithm &other);
  RandomWalkAlgorithm &operator=(const RandomWalkAlgorithm &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme);

  ~RandomWalkAlgorithm() {
    ClearGraph();
  }

 private:
  bool GenerateGraph();
  bool GenerateGraph(Window &window_handle, const ColorScheme &color_scheme);

  void GeneratePath();
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  /// \param position center of returned "square"
  /// \return neighbouring cells to cell under specified position
  std::vector<Coord> GenNeighbours(const Coord &position);

  Coord PopRandomCell(std::vector<Coord>& positions);

  bool SearchBreakingPoint(const std::vector<Coord>& possible_routs, bool& path_has_been_found);


  Cell &GetCell(const Coord &position) { return copy_plane_[position.ToInt(width_)]; };

  void ClearGraph();


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

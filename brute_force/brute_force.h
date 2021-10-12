//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_SAMPLE_H_
#define COA_SAMPLE_ALGORITHM_SAMPLE_H_
#include "../plane/plane.h"
#include "../sfml_window/window.h"
#include "../sfml_window/window_plane.h"
#include "../dijkstra/cell.h"


static void ConsoleDisplay(const std::vector<Coord> &path);

/// for now there only can be one start and one finish cell
class BruteForce {
 public:
  BruteForce() = delete;
  /// constructs Sample object based of Plane
  BruteForce(const Plane &other);
  BruteForce(const BruteForce &other);
  BruteForce &operator=(const BruteForce &other);

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath();

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme);

  ~BruteForce(){ClearGraph();}

 private:
  /// \param position center of returned "square"
  /// \return neighbouring cells to cell under specified position
  void GenNeighbours(const Coord &position, std::vector<Coord> &target_vec);

  bool GenerateGraph();
  void GeneratePath();

  bool GenerateGraph(Window &window_handle, const ColorScheme &color_scheme);
  void GeneratePath(Window &window_handle, const ColorScheme &color_scheme);

  Cell &GetCell(const Coord &position) { return copy_plane_[position.ToInt(width_)]; };

  bool SearchBreakingPoint(const std::vector<Coord> &possible_routs, bool &path_has_been_found);

  void ClearGraph();
  void UpdateGs();

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
static void ConsoleDisplay(const std::vector<Coord> &path) {
  if (path.size() > 15) {
    printf("path is to big, path size: %d\n", path.size());
    return;
  }
  if (path.empty()) {
    printf("there's no path\n");
    return;
  }
  for (int i = 0; i < path.size() -1 ; i++)
    printf("(%d,%d) -> ", path[i].x, path[i].y);
  printf("(%d,%d)\n", path[path.size() -1].x, path[path.size() -1].y);

}

#endif//COA_SAMPLE_ALGORITHM_SAMPLE_H_

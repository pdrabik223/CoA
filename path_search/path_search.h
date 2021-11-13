//
// Created by piotr on 19/10/2021.
//

#ifndef COA_PATH_SEARCH_PATH_SEARCH_H_
#define COA_PATH_SEARCH_PATH_SEARCH_H_
#include "a_star/a_star.h"
#include "depth_first/depth_first.h"
#include "dijkstra/dijkstra.h"
#include "greedy_best_first/greedy_best_first.h"
#include "greedy_pdistance/greedy_pdistance.h"
#include "random_walk/random_walk_algorithm.h"

#include "../maze_generator/maze_generator.h"

class PathSearch {

 public:
  PathSearch() = delete;

  PathSearch(const MazeGenerator &maze, Algorithm algorithm, Neighbourhood neighbourhood) {

    switch (algorithm) {
      case Algorithm::DIJKSTRA: search_engine_ = std::move(std::unique_ptr<GraphBase>(new Dijkstra(maze.GetPlane(), neighbourhood))); break;
      case Algorithm::A_STAR: search_engine_ = std::move(std::unique_ptr<GraphBase>(new AStar(maze.GetPlane(), neighbourhood))); break;
      case Algorithm::RANDOM_WALK: search_engine_ = std::move(std::unique_ptr<GraphBase>(new RandomWalk(maze.GetPlane(), neighbourhood))); break;
      case Algorithm::DEPTH_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new DepthFirst(maze.GetPlane(), neighbourhood))); break;
      case Algorithm::GREEDY_BEST_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(maze.GetPlane(), neighbourhood))); break;
      case Algorithm::GREEDY_P_DISTANCE: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyPDistance(maze.GetPlane(), neighbourhood))); break;
    }
  }

  PathSearch(const MazeGenerator &maze, Algorithm algorithm) {

    switch (algorithm) {
      case Algorithm::DIJKSTRA: search_engine_ = std::move(std::unique_ptr<GraphBase>(new Dijkstra(maze.GetPlane()))); break;
      case Algorithm::A_STAR: search_engine_ = std::move(std::unique_ptr<GraphBase>(new AStar(maze.GetPlane()))); break;
      case Algorithm::RANDOM_WALK: search_engine_ = std::move(std::unique_ptr<GraphBase>(new RandomWalk(maze.GetPlane()))); break;
      case Algorithm::DEPTH_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new DepthFirst(maze.GetPlane()))); break;
      case Algorithm::GREEDY_BEST_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(maze.GetPlane()))); break;
      case Algorithm::GREEDY_P_DISTANCE: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyPDistance(maze.GetPlane()))); break;
    }
  }

  PathSearch(const Plane &maze, Algorithm algorithm, Neighbourhood neighbourhood) {

    switch (algorithm) {
      case Algorithm::DIJKSTRA: search_engine_ = std::move(std::unique_ptr<GraphBase>(new Dijkstra(maze, neighbourhood))); break;
      case Algorithm::A_STAR: search_engine_ = std::move(std::unique_ptr<GraphBase>(new AStar(maze, neighbourhood))); break;
      case Algorithm::RANDOM_WALK: search_engine_ = std::move(std::unique_ptr<GraphBase>(new RandomWalk(maze, neighbourhood))); break;
      case Algorithm::DEPTH_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new DepthFirst(maze, neighbourhood))); break;
      case Algorithm::GREEDY_BEST_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(maze, neighbourhood))); break;
      case Algorithm::GREEDY_P_DISTANCE: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyPDistance(maze, neighbourhood))); break;
    }
  }

  PathSearch(const Plane &maze, Algorithm algorithm) {

    switch (algorithm) {
      case Algorithm::DIJKSTRA: search_engine_ = std::move(std::unique_ptr<GraphBase>(new Dijkstra(maze))); break;
      case Algorithm::A_STAR: search_engine_ = std::move(std::unique_ptr<GraphBase>(new AStar(maze))); break;
      case Algorithm::RANDOM_WALK: search_engine_ = std::move(std::unique_ptr<GraphBase>(new RandomWalk(maze))); break;
      case Algorithm::DEPTH_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new DepthFirst(maze))); break;
      case Algorithm::GREEDY_BEST_FIRST: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(maze))); break;
      case Algorithm::GREEDY_P_DISTANCE: search_engine_ = std::move(std::unique_ptr<GraphBase>(new GreedyPDistance(maze))); break;
    }
  }

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath() { return search_engine_->FindPath(); };

  /// find path from start to finish
  /// \return list of connected coordinates
  /// if returned list is empty the path does not exist
  std::vector<Coord> FindPath(Window &window_handle, const ColorScheme &color_scheme) { return search_engine_->FindPath(window_handle, color_scheme); };

  double TimePathSearch(std::vector<Coord> &path) {
    auto t_1 = std::chrono::high_resolution_clock::now();
    path = search_engine_->FindPath();
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count();
  }

 protected:
  std::unique_ptr<GraphBase> search_engine_;
};
static std::string ToString(Neighbourhood neighbourhood) {
  switch (neighbourhood) {

    case Neighbourhood::VON_NEUMAN: return "Von Neuman";
    case Neighbourhood::MOORE: return "Moore";
  }
}

#endif//COA_PATH_SEARCH_PATH_SEARCH_H_

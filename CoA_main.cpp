//
// Created by piotr on 04/10/2021.
//
#include "maze/maze_generator.h"
#include "path_search/path_search.h"
#include <iostream>
#include <memory>
#define WINDOW_SIZE 500

void MessageMe(int maze_nr, std::pair<MazeType, Algorithm> settings, size_t time, int path_length) {

  std::string maze_type = ToString(settings.first);
  std::string algorithm = ToString(settings.second);

  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}

#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()

class Generator {
 public:
  Generator(std::pair<MazeType, Algorithm> settings, Window &window, const ColorScheme &color_scheme) {

    generator_thread_ = new std::thread(&Generator::MainLoop, this, settings, std::ref(window), color_scheme);
  };

  void MainLoop(std::pair<MazeType, Algorithm> settings, Window &window, ColorScheme color_scheme) {
    std::chrono::steady_clock::time_point t_1;
    double time;

    for (int i = 0; i < 10; ++i) {
      MazeGenerator maze(100, 100, settings.first);
      std::unique_ptr<GraphBase> engine;

      switch (settings.second) {
        case Algorithm::DIJKSTRA: engine = std::move(std::unique_ptr<GraphBase>(new Dijkstra(maze.GetPlane()))); break;
        case Algorithm::A_STAR: engine = std::move(std::unique_ptr<GraphBase>(new AStar(maze.GetPlane()))); break;
        case Algorithm::RANDOM_WALK: engine = std::move(std::unique_ptr<GraphBase>(new RandomWalk(maze.GetPlane()))); break;
        case Algorithm::RIGHT_HAND_RULE: engine = std::move(std::unique_ptr<GraphBase>(new RHR(maze.GetPlane()))); break;
        case Algorithm::DEPTH_FIRST: engine = std::move(std::unique_ptr<GraphBase>(new DepthFirst(maze.GetPlane()))); break;
        case Algorithm::GREEDY_BEST_FIRST: engine = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(maze.GetPlane()))); break;
        case Algorithm::GREEDY_P_DISTANCE: engine = std::move(std::unique_ptr<GraphBase>(new GreedyPDistance(maze.GetPlane()))); break;
      }

      auto path = engine->FindPath(window, color_scheme);
      t_1 = T_START;
      engine->FindPath();
      time = T_RECORD(t_1);
      MessageMe(i, settings, time, path.size());
    }
  };

  ~Generator() {
    generator_thread_->join();
    delete generator_thread_;
  }

  std::thread *generator_thread_;
};

void GlobalVisuals(std::vector<std::pair<MazeType, Algorithm>> settings) {

  std::vector<Window *> windows;
  windows.reserve(settings.size());
  std::vector<Generator> generators;
  generators.reserve(settings.size());

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();

  for (int i = 0; i < settings.size(); ++i) {

    windows.push_back(new Window({{i * WINDOW_SIZE, 0}, WINDOW_SIZE, WINDOW_SIZE}));

    generators.emplace_back(settings[i], *windows[i], color_scheme);
  }
}

int main() {
  srand(time(NULL));

  std::vector<std::pair<MazeType, Algorithm>> settings = {{MazeType::PLANE_10, Algorithm::DIJKSTRA},
                                                          {MazeType::SNAIL_MAZE, Algorithm::A_STAR},
                                                          {MazeType::CIRCUlAR_MAZE, Algorithm::DIJKSTRA}};
  GlobalVisuals(settings);

  return 0;
}
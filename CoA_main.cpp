//
// Created by piotr on 04/10/2021.
//
#include "maze/maze_generator.h"
#include "path_search/path_search.h"
#include <iostream>
#define WINDOW_SIZE 500

#define RUN_BRUTEFORCE true
#define RUN_RANDOM_WALK false
#define RUN_DIJKSTRA false

#define USE_RANDOM_GEN true
#define USE_SQUARE_MAZE false
#define USE_CIRCULAR_MAZE false
enum class MazeType {
  EMPTY_PLANE,
  PLANE_5,
  PLANE_10,
  PLANE_20,
  PLANE_25,
  CIRCUlAR_MAZE,
  SQUARE_MAZE,
  SNAIL_MAZE
};
enum class Algorythm {
  DIJKSTRA,
  A_STAR,
  RANDOM_WALK
};

void MessageMe(int maze_nr, std::pair<MazeType, Algorythm> settings, size_t time, int path_length) {

  std::string maze_type;
  std::string algorithm;
  switch (settings.first) {
    case MazeType::EMPTY_PLANE: maze_type = "empty plane"; break;
    case MazeType::PLANE_5:
      maze_type = "infill 5%";
      break;
    case MazeType::PLANE_10:
      maze_type = "infill 10%";
      break;
    case MazeType::PLANE_20:
      maze_type = "infill 20%";
      break;
    case MazeType::PLANE_25:
      maze_type = "infill 25%";
      break;
    case MazeType::CIRCUlAR_MAZE:
      maze_type = "circular maze";
      break;
    case MazeType::SQUARE_MAZE:
      maze_type = "square maze";
      break;
    case MazeType::SNAIL_MAZE:
      maze_type = "snail maze";
      break;
  }

  switch (settings.second) {
    case Algorythm::DIJKSTRA:
      algorithm = "Dijkstra";
      break;
    case Algorythm::A_STAR: algorithm = "A*"; break;
    case Algorythm::RANDOM_WALK: algorithm = "Random Walk"; break;
  }
  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}

#define T_START std::chrono::high_resolution_clock::now()
#define T_RECORD(t_1) std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - (t_1)).count()

class Generator {
 public:
  Generator(std::pair<MazeType, Algorythm> settings, Window &window, const ColorScheme &color_scheme) {

    generator_thread_ = new std::thread(&Generator::MainLoop, this, settings, std::ref(window), color_scheme);
  };

  void MainLoop(std::pair<MazeType, Algorythm> settings, Window &window, ColorScheme color_scheme) {
    std::chrono::steady_clock::time_point t_1;
    double time = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    for (int i = 0; i < 10; ++i) {
      MazeGenerator maze(100, 100);
      switch (settings.first) {
        case MazeType::EMPTY_PLANE:
          maze.GenRandomMaze(0);
          break;
        case MazeType::PLANE_5:
          maze.GenRandomMaze(5);
          break;
        case MazeType::PLANE_10:
          maze.GenRandomMaze(10);
          break;
        case MazeType::PLANE_20:
          maze.GenRandomMaze(20);
          break;
        case MazeType::PLANE_25:
          maze.GenRandomMaze(25);
          break;
        case MazeType::CIRCUlAR_MAZE:
          maze.GenCircularMaze();
          break;
        case MazeType::SQUARE_MAZE:
          maze.GenSquareMaze();
          break;
        case MazeType::SNAIL_MAZE:
          maze.GenSnailMaze();
          break;
      }

      switch (settings.second) {
        case Algorythm::DIJKSTRA: {
          Dijkstra dijstra(maze.GetPlane());
          auto path = dijstra.FindPath(window, color_scheme);
          t_1 = T_START;
          dijstra.FindPath();
          time = T_RECORD(t_1);
          MessageMe(i, settings, time, path.size());
        } break;
        case Algorythm::A_STAR: {
          AStar a_star(maze.GetPlane());
          auto path = a_star.FindPath(window, color_scheme);
          t_1 = T_START;
          a_star.FindPath();
          time = T_RECORD(t_1);
          MessageMe(i, settings, time, path.size());

        } break;
        case Algorythm::RANDOM_WALK: {
          RandomWalk random_walk(maze.GetPlane());
          auto path = random_walk.FindPath(window, color_scheme);
          t_1 = T_START;
          random_walk.FindPath();
          time = T_RECORD(t_1);
          MessageMe(i, settings, time, path.size());
        } break;
      }
    }
  };

  ~Generator() {
    generator_thread_->join();
    delete generator_thread_;
  }

  std::thread *generator_thread_;
};

void GlobalVisuals(std::vector<std::pair<MazeType, Algorythm>> settings) {

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

  std::vector<std::pair<MazeType, Algorythm>> settings = {{MazeType::SQUARE_MAZE, Algorythm::A_STAR},
                                                          {MazeType::CIRCUlAR_MAZE, Algorythm::DIJKSTRA},
                                                          {MazeType::SQUARE_MAZE, Algorythm::A_STAR}};
  GlobalVisuals(settings);

  return 0;
}
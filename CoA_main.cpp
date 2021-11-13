//
// Created by piotr on 04/10/2021.
//
#include "../maze_generator/maze_generator.h"
#include "path_search/path_search.h"
#include <iostream>
#include <memory>
#include <tuple>
#define WINDOW_SIZE 500

void MessageMe(int maze_nr, std::tuple<MazeType, Algorithm, Neighbourhood> settings, size_t time, int path_length) {

  std::string maze_type = ToString(std::get<0>(settings));
  std::string algorithm = ToString(std::get<1>(settings));
  std::string neighbourhood = ToString(std::get<2>(settings));

  std::cout << "maze nr: " << maze_nr << "\tmaze type: " << maze_type << "\tneighbourhood: " << neighbourhood << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}
class Generator {
 public:
  Generator(std::tuple<MazeType, Algorithm, Neighbourhood> settings, Window &window, const ColorScheme &color_scheme) {

    generator_thread_ = new std::thread(&Generator::MainLoop, this, settings, std::ref(window), color_scheme);
  };

  void MainLoop(std::tuple<MazeType, Algorithm, Neighbourhood> settings, Window &window, ColorScheme color_scheme) {
    double time;

    for (int i = 0; i < 10; ++i) {
      MazeGenerator maze(100, 100, std::get<0>(settings));
      PathSearch engine(maze, std::get<1>(settings), std::get<2>(settings));

      auto path = engine.FindPath(window, color_scheme);

      time = engine.TimePathSearch(path);

      MessageMe(i, settings, time, path.size());
    }
  };

  ~Generator() {
    generator_thread_->join();
    delete generator_thread_;
  }

  std::thread *generator_thread_;
};

void GlobalVisuals(std::vector<std::tuple<MazeType, Algorithm, Neighbourhood>> settings) {

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

  std::vector<std::tuple<MazeType, Algorithm, Neighbourhood>> settings = {
      //      {MazeType::EMPTY_PLANE, Algorithm::DEPTH_FIRST},
      {MazeType::EMPTY_PLANE, Algorithm::DIJKSTRA, Neighbourhood::VON_NEUMAN},
      {MazeType::EMPTY_PLANE, Algorithm::DIJKSTRA, Neighbourhood::MOORE}};
  GlobalVisuals(settings);

  return 0;
}
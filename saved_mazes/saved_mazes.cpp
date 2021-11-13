//
// Created by piotr on 12/10/2021.
//

#include "../path_search/path_search.h"

#include <iostream>
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
int main() {

  Plane cos("../saved_mazes/0.txt");

  ColorScheme color_scheme;
  color_scheme.LoadGreenSet();
  Window screen(800, 800);
  Generator main({cos, Algorithm::RANDOM_WALK}, screen, color_scheme);

  return 0;
}
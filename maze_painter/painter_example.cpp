//
// Created by piotr on 09/10/2021.
//
#include "../path_search/path_search.h"

#include "maze_painter.h"
#include <conio.h>
#include <iostream>
#define WINDOW_SIZE 500

void MessageMe(std::tuple<Algorithm, Neighbourhood> settings, size_t time, int path_length) {

  std::string algorithm = ToString(std::get<0>(settings));
  std::string neighbourhood = ToString(std::get<1>(settings));

  std::cout << "\tneighbourhood: " << neighbourhood << "\talgorithm: " << algorithm << "\ttime:" << time << "us\t"
            << "path length: " << path_length << "\n";
}

class Generator {
 public:
  Generator(std::tuple<Plane, Algorithm, Neighbourhood> settings, Window &window, const ColorScheme &color_scheme) {

    generator_thread_ = new std::thread(&Generator::MainLoop, this, settings, std::ref(window), color_scheme);
  };

  void MainLoop(std::tuple<Plane, Algorithm, Neighbourhood> settings, Window &window, ColorScheme color_scheme) {
    double time;

    PathSearch engine(std::get<0>(settings), std::get<1>(settings), std::get<2>(settings));

    auto path = engine.FindPath(window, color_scheme);

    time = engine.TimePathSearch(path);

    MessageMe({std::get<1>(settings), std::get<2>(settings)}, time, path.size());
  };

  ~Generator() {
    generator_thread_->join();
    delete generator_thread_;
  }

  std::thread *generator_thread_;
};

void GlobalVisuals(std::vector<std::tuple<Plane, Algorithm, Neighbourhood>> &settings) {

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

  MazePainter maze(800, 800, 80, 80);
  maze.MainLoop();
  maze.GetPlane().SaveToFile("../saved_mazes/test.txt");

  std::vector<std::tuple<Plane, Algorithm, Neighbourhood>> settings = {
      //      {maze.GetPlane(), Algorithm::GREEDY_BEST_FIRST, Neighbourhood::MOORE},
      //      {maze.GetPlane(), Algorithm::A_STAR, Neighbourhood::MOORE},
      {maze.GetPlane(), Algorithm::A_STAR, Neighbourhood::MOORE}};

  GlobalVisuals(settings);
  getch();

  return 0;
}
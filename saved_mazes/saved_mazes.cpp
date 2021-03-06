//
// Created by piotr on 12/10/2021.
//

#include "../path_search/path_search.h"

#include <iostream>
#define WINDOW_SIZE 500

class Generator {
 public:
  Generator(std::pair<Plane, Algorithm> settings, Window &window, const ColorScheme &color_scheme) {

    generator_thread_ = new std::thread(&Generator::MainLoop, this, settings, std::ref(window), color_scheme);
  };

  void MainLoop(std::pair<Plane, Algorithm> settings, Window &window, ColorScheme color_scheme) {

    std::unique_ptr<GraphBase> engine;

    switch (settings.second) {
      case Algorithm::DIJKSTRA: engine = std::move(std::unique_ptr<GraphBase>(new Dijkstra(settings.first))); break;
      case Algorithm::A_STAR: engine = std::move(std::unique_ptr<GraphBase>(new AStar(settings.first))); break;
      case Algorithm::RANDOM_WALK: engine = std::move(std::unique_ptr<GraphBase>(new RandomWalk(settings.first))); break;
      case Algorithm::RIGHT_HAND_RULE: engine = std::move(std::unique_ptr<GraphBase>(new RHR(settings.first))); break;
      case Algorithm::DEPTH_FIRST: engine = std::move(std::unique_ptr<GraphBase>(new DepthFirst(settings.first))); break;
      case Algorithm::GREEDY_BEST_FIRST: engine = std::move(std::unique_ptr<GraphBase>(new GreedyBestFirst(settings.first))); break;
    }

    auto path = engine->FindPath(window, color_scheme);
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
  Generator main({cos, Algorithm::RIGHT_HAND_RULE}, screen, color_scheme);

  return 0;
}
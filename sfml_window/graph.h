//
// Created by piotr on 11/10/2021.
//

#ifndef COA_SFML_WINDOW_GRAPH_H_
#define COA_SFML_WINDOW_GRAPH_H_

#include "../utility/coord.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <mutex>
#include <string>
#include <vector>
#define BACKGROUND_COLOR sf::Color::White

class Graph {

  using Datapoint = std::pair<int, int>;

 public:
  Graph(unsigned int window_width, unsigned int window_height) : window_width_(window_width), window_height_(window_height) {}

  Graph(const Graph &other);
  Graph &operator=(const Graph &other);

  void PushData(int x_value, int y_value);

  void Draw(sf::RenderWindow &window);

  void MainLoop();

 protected:
  unsigned window_width_ = 800;
  unsigned window_height_ = 1000;
  std::string graph_label_ = "CoA - graph tool";
  std::string y_label_;
  std::string x_label_;
  std::mutex data_mutex_;
  std::vector<Datapoint> data_;
  std::vector<sf::Color> data_colors_;
  sf::Event event_;
};

#endif//COA_SFML_WINDOW_GRAPH_H_

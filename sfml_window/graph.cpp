//
// Created by piotr on 11/10/2021.
//

#include "graph.h"
void Graph::MainLoop() {

  sf::ContextSettings settings;
  settings.antialiasingLevel = 10;

  sf::RenderWindow window(sf::VideoMode(window_width_, window_height_), graph_label_,
                          sf::Style::Default, settings);

  window.setPosition(sf::Vector2i(0, 0));
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  // window.display();
  sf::Clock clock;

  while (window.isOpen()) {

    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (window.pollEvent(event_)) {
      if (event_.key.code == sf::Keyboard::Escape) {
        window.close();
      } else if (event_.key.code == sf::Keyboard::R) {
        Draw(window);
      }
    }
    //      Draw(window);
  }
}
void Graph::Draw(sf::RenderWindow &window) {
  window.clear(BACKGROUND_COLOR);
  //  std::lock_guard<std::mutex> lock(data_mutex_);
  if (data_.empty()) return;

  int max_x = (int) 0;
  int max_y = (int) 0;

  for (auto &d : data_) {
    if (d.first > max_x) max_x = d.first;
    if (d.second > max_y) max_y = d.first;
  }

  Coord graph_border_px = {20, 10};
  // Coord screen_size = {(int) window_width_ - graph_border_px.x, (int) window_height_ - graph_border_px.y};
  double point_size = 3;
  sf::CircleShape point;
  point.setRadius(point_size);
  for (int i = 0; i < data_.size(); i++) {
    point.setPosition((data_[i].first), (window_height_ - data_[i].second));
    point.setFillColor(data_colors_[i]);
    window.draw(point);
  }

  window.display();
}
void Graph::PushData(int x_value, int y_value) {
  std::lock_guard<std::mutex> lock(data_mutex_);
  data_.emplace_back(x_value, y_value);
  data_colors_.push_back(sf::Color::Red);
  event_.key.code = sf::Keyboard::R;
}
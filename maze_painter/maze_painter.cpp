//
// Created by piotr on 09/10/2021.
//

#include "maze_painter.h"
#include "../sfml_window/window_plane.h"

#include <SFML/Graphics/RenderWindow.hpp>
MazePainter::MazePainter(int screen_width, int screen_height, int plane_width, int plane_height) : screen_width_(screen_width), screen_height_(screen_height), plane_(plane_width, plane_height, 0), color_scheme_() {
  float cell_width = screen_width / plane_width;
  float cell_height = screen_height / plane_height;

  cell_size_ = cell_height < cell_width ? cell_height : cell_width;

  color_scheme_.LoadGreenSet();
}
const Plane &MazePainter::GetPlane() const {
  return plane_;
}
void MazePainter::MainLoop() {
  sf::ContextSettings settings;
  //  settings.antialiasingLevel = 10;

  sf::RenderWindow window(sf::VideoMode(screen_width_, screen_height_), "CoA",
                          sf::Style::Default, settings);

  window.setPosition(sf::Vector2i(0, 0));

  DrawPlane(window);
  window.display();
  sf::Clock clock;

  while (window.isOpen()) {

    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (window.waitEvent(event_)) {
      if (event_.type == sf::Event::Closed)
        window.close();
      else if (event_.type == sf::Event::KeyPressed) {
        if (event_.key.code == sf::Keyboard::F) {

          // adds finish point at cursor position
          int m_px_pos_x = sf::Mouse::getPosition(window).y / cell_size_;
          int m_px_pos_y = sf::Mouse::getPosition(window).x / cell_size_;
          if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
            plane_.SetCell(Coord(Coord(m_px_pos_x, m_px_pos_y)), CellState::FINISH);
            DrawPlane(window);
          }
        } else if (event_.key.code == sf::Keyboard::S) {
          // adds start point at cursor position
          int m_px_pos_x = sf::Mouse::getPosition(window).y / cell_size_;
          int m_px_pos_y = sf::Mouse::getPosition(window).x / cell_size_;
          if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
            plane_.SetCell(Coord(Coord(m_px_pos_x, m_px_pos_y)), CellState::START);
            DrawPlane(window);
          }
        }
      } else if (event_.type == sf::Event::MouseMoved) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

          int m_px_pos_x = sf::Mouse::getPosition(window).y / cell_size_;
          int m_px_pos_y = sf::Mouse::getPosition(window).x / cell_size_;
          if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
            plane_.SetCell(Coord(m_px_pos_x, m_px_pos_y), CellState::WALL);
//            DrawPlane(window);
          }
        } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

          int m_px_pos_x = sf::Mouse::getPosition(window).y / cell_size_;
          int m_px_pos_y = sf::Mouse::getPosition(window).x / cell_size_;
          if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
            plane_.SetCell(Coord(m_px_pos_x, m_px_pos_y), CellState::EMPTY);
//            DrawPlane(window);
          }
        }
      }
      DrawPlane(window);
    }

    DrawPlane(window);
  }
}
void MazePainter::DrawPlane(sf::RenderWindow &window) {
  WindowPlane screen_texture(plane_, color_scheme_);
  std::vector<Coord> highlights;

  int m_px_pos_x = sf::Mouse::getPosition(window).x / cell_size_;
  int m_px_pos_y = sf::Mouse::getPosition(window).y / cell_size_;

  if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
    highlights.emplace_back(m_px_pos_x, m_px_pos_y);
    screen_texture.HighlightCells(highlights);
  }

  screen_texture.DrawToWindow(window);
  window.display();
}

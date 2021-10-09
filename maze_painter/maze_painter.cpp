//
// Created by piotr on 09/10/2021.
//

#include "maze_painter.h"
#include "../sfml_window/window_plane.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <iostream>
MazePainter::MazePainter(int screen_width, int screen_height, int plane_width, int plane_height) : screen_width_(screen_width), screen_height_(screen_height), plane_(plane_width, plane_height, 0), color_scheme_() {
  float cell_width = screen_width / plane_width;
  float cell_height = screen_height / plane_height;

  cell_size_ = cell_height < cell_width ? cell_height : cell_width;

  color_scheme_.LoadPainterSet();
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

  Coord mouse_position;
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (window.waitEvent(event_)) {
      if (event_.type == sf::Event::Closed)
        window.close();
      else if (event_.type == sf::Event::KeyPressed) {
        if (event_.key.code == sf::Keyboard::F) {

          current_brush_state_ = CellState::FINISH;
        } else if (event_.key.code == sf::Keyboard::S) {
          current_brush_state_ = CellState::START;
        } else if (event_.key.code == sf::Keyboard::W) {
          current_brush_state_ = CellState::WALL;
        } else if (event_.key.code == sf::Keyboard::Up) {
          brush_size_++;
        } else if (event_.key.code == sf::Keyboard::Down) {
          if (brush_size_ > 0) brush_size_--;
        }

      }

      else if (event_.type == sf::Event::MouseMoved) {
        DrawPlane(window);
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

        if (PopulateMousePosition(window, mouse_position)) {
          std::vector<Coord> painted_squares;
          DrawFilledInCircle(mouse_position, painted_squares);
          for (auto &ps : painted_squares)
            plane_.SetCell(ps, current_brush_state_);

          DrawPlane(window);
        }

      } else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        if (PopulateMousePosition(window, mouse_position)) {

          std::vector<Coord> painted_squares;
          DrawFilledInCircle(mouse_position, painted_squares);

          for (auto &ps : painted_squares)
            plane_.SetCell(ps, CellState::EMPTY);

          DrawPlane(window);
        }
      }

    }
  DrawPlane(window);
  }


window.close();
}
void MazePainter::DrawPlane(sf::RenderWindow &window) {
  WindowPlane screen_texture(plane_, color_scheme_);

  switch (current_brush_state_) {

    case CellState::EMPTY:
      screen_texture.HighlightCells(GetHighlightedPositions(window), color_scheme_.background);
      break;
    case CellState::WALL:
      screen_texture.HighlightCells(GetHighlightedPositions(window), color_scheme_.wall);
      break;
    case CellState::START:
      screen_texture.HighlightCells(GetHighlightedPositions(window), color_scheme_.start);
      break;
    case CellState::FINISH:
      screen_texture.HighlightCells(GetHighlightedPositions(window), color_scheme_.finish);
      break;
  }

  screen_texture.DrawToWindow(window);
  window.display();
}

bool MazePainter::PopulateMousePosition(const sf::RenderWindow &window, Coord &mouse_position) {

  int m_px_pos_x = sf::Mouse::getPosition(window).y / cell_size_;
  int m_px_pos_y = sf::Mouse::getPosition(window).x / cell_size_;
  if (m_px_pos_x >= 0 and m_px_pos_x < plane_.GetWidth() and m_px_pos_y >= 0 and m_px_pos_y < plane_.GetHeight()) {
    mouse_position = {m_px_pos_x, m_px_pos_y};
    return true;
  }
  return false;
}
std::vector<Coord> MazePainter::GetHighlightedPositions(const sf::RenderWindow &window) {
  Coord mouse;

  if (not PopulateMousePosition(window, mouse)) return {};

  std::vector<Coord> highlights;
  DrawFilledInCircle(mouse, highlights);

  return highlights;
}
void MazePainter::CheckBoundariesAndPush(const Coord &position, std::vector<Coord> &push_target) {
  if (position.x >= 0 and position.x < plane_.GetWidth() and position.y >= 0 and position.y < plane_.GetHeight()) {
    push_target.push_back(position);
  }
}
void MazePainter::DrawCircle(const Coord &center, std::vector<Coord> &target, int radius) {
  int x = radius;
  int y = 0;
  int err = 0;

  while (x >= y) {
    CheckBoundariesAndPush({center.y + y, center.x + x}, target);
    CheckBoundariesAndPush({center.y + x, center.x + y}, target);
    CheckBoundariesAndPush({center.y + x, center.x - y}, target);
    CheckBoundariesAndPush({center.y + y, center.x - x}, target);
    CheckBoundariesAndPush({center.y - y, center.x - x}, target);
    CheckBoundariesAndPush({center.y - x, center.x - y}, target);
    CheckBoundariesAndPush({center.y - x, center.x + y}, target);
    CheckBoundariesAndPush({center.y - y, center.x + x}, target);
    if (err <= 0) {
      ++y;
      err += 2 * y + 1;
    } else {
      --x;
      err -= 2 * x + 1;
    }
  }
}
void MazePainter::DrawFilledInCircle(const Coord &center, std::vector<Coord> &target) {
  for (int r = 0; r < brush_size_; r++) {
    DrawCircle(center, target, r);
  }
}

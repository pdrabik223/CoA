//
// Created by piotr on 09/10/2021.
//

#ifndef COA_MAZE_PAINTER_MAZE_PAINTER_H_
#define COA_MAZE_PAINTER_MAZE_PAINTER_H_

#include "../plane/plane.h"
#include "../sfml_window/color_scheme.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

/// All in one window and painter logic
class MazePainter {
 public:
  MazePainter() = delete;
  MazePainter(int screen_width, int screen_height, int plane_width, int plane_height);

  /// main window loop
  void MainLoop();
  void DrawPlane(sf::RenderWindow &window);
  const Plane &GetPlane() const;


 protected:
  int screen_width_;
  int screen_height_;

  float cell_size_;

  ColorScheme color_scheme_;

  sf::Event event_;
  Plane plane_;
};

#endif//COA_MAZE_PAINTER_MAZE_PAINTER_H_

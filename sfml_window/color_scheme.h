//
// Created by piotr on 06/10/2021.
//

#ifndef COA_SFML_WINDOW_COLOR_SHEME_H_
#define COA_SFML_WINDOW_COLOR_SHEME_H_

#include <SFML/Graphics/Color.hpp>
struct ColorScheme {

  ColorScheme();
  ColorScheme(const ColorScheme& other ) = default;
  ColorScheme& operator=(const ColorScheme& other) = default;
  ColorScheme(const sf::Color &background, const sf::Color &wall, const sf::Color &discovered, const sf::Color &point_of_interest, const sf::Color &start, const sf::Color &finish);

  void LoadCyanSet();
  void LoadGreenSet();
  void LoadOrangeSet();
  void LoadPainterSet();

  sf::Color background;
  sf::Color wall;
  sf::Color discovered;
  sf::Color point_of_interest;
  sf::Color start;
  sf::Color finish;
  sf::Color path;
};

#endif//COA_SFML_WINDOW_COLOR_SHEME_H_

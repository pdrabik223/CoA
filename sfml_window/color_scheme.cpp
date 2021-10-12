//
// Created by piotr on 06/10/2021.
//

#include "color_scheme.h"

ColorScheme::ColorScheme(const sf::Color &background, const sf::Color &wall, const sf::Color &discovered, const sf::Color &point_of_interest, const sf::Color &start, const sf::Color &finish) : background(background), wall(wall), discovered(discovered), point_of_interest(point_of_interest), start(start), finish(finish) {}
ColorScheme::ColorScheme() : background(25, 25, 25), wall(sf::Color::Black), discovered(sf::Color::Cyan), point_of_interest(sf::Color::Magenta), start(sf::Color::Yellow), finish(sf::Color::Red) {}
void ColorScheme::LoadCyanSet() {
  background = sf::Color(32, 247, 236);
  wall = sf::Color(63, 72, 204);
  discovered = sf::Color(150, 252, 58);
  point_of_interest = sf::Color(255, 215, 0);
  start = sf::Color(255, 0, 0);
  finish = sf::Color(255, 255, 255);
  path = sf::Color(255, 215, 0);
}
void ColorScheme::LoadGreenSet() {
  background = sf::Color(255, 255, 255);
  //  wall = sf::Color(0x187e8a);

  wall = sf::Color(126, 0, 255);

  discovered = sf::Color(0xdb3934);

  //  point_of_interest = sf::Color(0xff0000);

  point_of_interest = sf::Color(sf::Color::Red);

  start = sf::Color(sf::Color::Red);
  finish = sf::Color(255, 215, 0);
  path = sf::Color(255, 215, 0);
}
void ColorScheme::LoadOrangeSet() {
  background = sf::Color(0x323031);
  discovered = sf::Color(0xF9A32A);
  wall = sf::Color(0x842af9);
  point_of_interest = sf::Color(0x77ef21);

  start = sf::Color(sf::Color::Red);
  finish = sf::Color(sf::Color::White);
  path = sf::Color(255, 215, 0);
}
void ColorScheme::LoadPainterSet() {
  background = sf::Color(29, 38, 38);
  discovered = sf::Color(249, 163, 42);
  wall = sf::Color(132, 42, 249);
  point_of_interest = sf::Color(119, 239, 33);

  start = sf::Color(sf::Color::Red);
  finish = sf::Color(255, 215, 0);
  path = sf::Color(255, 215, 0);
}

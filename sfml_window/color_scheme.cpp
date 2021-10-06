//
// Created by piotr on 06/10/2021.
//

#include "color_scheme.h"
ColorScheme::ColorScheme(const sf::Color &background, const sf::Color &wall, const sf::Color &discovered, const sf::Color &point_of_interest, const sf::Color &start, const sf::Color &finish) : background(background), wall(wall), discovered(discovered), point_of_interest(point_of_interest), start(start), finish(finish) {}
ColorScheme::ColorScheme(): background(25,25,25), wall(sf::Color::Black), discovered(sf::Color::Cyan), point_of_interest(sf::Color::Magenta), start(sf::Color::Yellow), finish(sf::Color::Red) {}
void ColorScheme::LoadCyanSet() {
  background = sf::Color(32,247,236);
  wall = sf::Color(63,72,204);
  discovered = sf::Color(150,252,58);
  point_of_interest = sf::Color(255,215,0);
  start = sf::Color(255,0,0);
  finish = sf::Color(255,255,255);
}
void ColorScheme::LoadRedSet() {
  background = sf::Color(50,48,49);
  wall = sf::Color(24,126,138);

  discovered = sf::Color(219,57,52);

  point_of_interest = sf::Color(255,200,87);

  start = sf::Color(255,0,0);
  finish = sf::Color(255,255,255);
}

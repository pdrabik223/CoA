//
// Created by piotr on 04/10/2021.
//

#include "plane.h"
Plane::Plane(const Plane &other) {
  width_ = other.width_;
  height_ = other.width_;

  for(auto &c:other.plane_)
    plane_.emplace_back(c);
}
Plane &Plane::operator=(const Plane &other) {
  if(&other == this) return *this;
  width_ = other.width_;
  height_ = other.width_;

  for(auto &c:other.plane_)
    plane_.emplace_back(c);


  return *this;
}
uint8_t Plane::GetCell(const Coord &position) {
  return plane_[position.ToInt(width_)];
}
void Plane::SetCell(const Coord &position, uint8_t value) {
  plane_[position.ToInt(width_)] = value;
}
unsigned int Plane::GetWidth() const {
  return width_;
}
unsigned int Plane::GetHeight() const {
  return height_;
}

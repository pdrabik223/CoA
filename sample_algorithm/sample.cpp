//
// Created by piotr on 05/10/2021.
//

#include "sample.h"
Sample::Sample(const Plane &other) : width_(other.GetWidth()), height_(other.GetHeight()) {
  copy_plane_.reserve(other.GetHeight() * other.GetWidth());

  for (int y = 0; y < other.GetHeight(); y++)
    for (int x = 0; x < other.GetWidth(); x++)
      copy_plane_.emplace_back(other.GetCell({x, y}));
}

Sample::Sample(const Sample &other) : width_(other.width_), height_(other.height_) {
  copy_plane_.reserve(width_ * height_);
  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);
}
Sample &Sample::operator=(const Sample &other) {

  if (&other == this) return *this;

  width_ = other.width_;
  height_ = other.height_;
  copy_plane_.reserve(width_ * height_);

  for (int i = 0; i < width_ * height_; i++)
    copy_plane_.emplace_back(other.copy_plane_[i]);

  return *this;
}

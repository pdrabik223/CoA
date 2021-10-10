//
// Created by piotr on 04/10/2021.
//

#include "plane.h"
#include <fstream>
Plane::Plane(const Plane &other) {
  width_ = other.width_;
  height_ = other.width_;

  for (auto &c : other.plane_)
    plane_.emplace_back(c);
}
Plane &Plane::operator=(const Plane &other) {
  if (&other == this) return *this;
  width_ = other.width_;
  height_ = other.width_;

  for (auto &c : other.plane_)
    plane_.emplace_back(c);

  return *this;
}
CellState Plane::GetCell(const Coord &position) const {
  return plane_[position.ToInt(width_)];
}
void Plane::SetCell(const Coord &position, const CellState value) {
  plane_[position.ToInt(width_)] = value;
}
unsigned int Plane::GetWidth() const {
  return width_;
}
unsigned int Plane::GetHeight() const {
  return height_;
}
Plane::Plane(unsigned int width, unsigned int height, int infill_percentage) : width_(width), height_(height) {

  plane_.reserve(width * height);

  for (int i = 0; i < width * height; i++)
    if (rand() % 100 < infill_percentage)
      plane_.emplace_back(CellState::WALL);
    else
      plane_.emplace_back(CellState::EMPTY);
}
void Plane::AddBorder() {
  //vertical top
  for (int x = 0; x < GetWidth(); x++)
    SetCell({x, 0}, CellState::WALL);

  //vertical bottom
  for (int x = 0; x < GetWidth(); x++)
    SetCell({x, (int) GetHeight() - 1}, CellState::WALL);

  // left edge
  for (int y = 0; y < GetHeight(); y++)
    SetCell({0, y}, CellState::WALL);

  // right ege
  for (int y = 0; y < GetHeight(); y++)
    SetCell({(int) GetWidth() - 1, y}, CellState::WALL);
}
void Plane::SaveToFile(const std::string &file_name) const {
  std::ofstream myfile;
  myfile.open(file_name, std::ios::out);
  if (not myfile.is_open()) throw "invalid filename";

  myfile << height_ << "\n";
  myfile << height_ << "\n";
  for (auto &c : plane_) {
    myfile << (int) c << " ";
  }
  myfile.close();
}
void Plane::LoadFromFile(const std::string &file_name) {
  std::ifstream myfile;
  myfile.open(file_name, std::ios::in);
  if (not myfile.is_open()) throw "invalid filename";
  myfile >> height_;
  myfile >> width_;
  int buffer;

  for (int c = 0; c < height_ * width_; ++c) {
    myfile >> buffer;
    plane_.emplace_back((CellState) buffer);
  }
  myfile.close();
}
void Plane::Clear() {
  for (auto &c : plane_) {
    c = CellState::EMPTY;
  }
}

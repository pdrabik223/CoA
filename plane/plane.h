//
// Created by piotr on 04/10/2021.
//

#ifndef COA_PLANE_H
#define COA_PLANE_H
#include "../utility/coord.h"
#include <ciso646>
#include <vector>

class Plane {
 public:
  Plane():width_(0),height_(0){};
  Plane(const Plane &other);
  Plane &operator=(const Plane &other);

  uint8_t GetCell(const Coord &position);
  void SetCell(const Coord &position, uint8_t value);

  unsigned int GetWidth() const;
  unsigned int GetHeight() const;


 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<uint8_t> plane_;
};

#endif//COA_PLANE_H

//
// Created by piotr on 05/10/2021.
//

#include "window_plane.h"
WindowPlane::WindowPlane(const Plane &plane) : width_(plane.GetWidth()), height_(plane.GetHeight()) {
  for (int x = 0; x < width_; x++)
    for (int y = 0; y < height_; y++) {

      switch (plane.GetCell({x, y})) {

      }
    }
}

//
// Created by piotr on 04/10/2021.
//

#ifndef COA_PLANE_H
#define COA_PLANE_H
#include "../utility/coord.h"
#include <ciso646>
#include <vector>

enum class CellType {
  EMPTY,
  WALL,
  START,
  FINISH
};

class Plane {
 public:
  Plane() : width_(0), height_(0){};
  Plane(unsigned int width, unsigned int height);
  Plane(const Plane &other);
  Plane &operator=(const Plane &other);

  CellType GetCell(const Coord &position) const;
  void SetCell(const Coord &position, CellType value);

  unsigned int GetWidth() const;
  unsigned int GetHeight() const;

  void AddBorder();


 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<CellType> plane_;
};
static void ConsoleDisplay(const Plane &target) {
  if (target.GetWidth() > 30 or target.GetHeight() > 30) {

    printf("to big plane\n");
    return;
  }


  for (int y = 0; y < target.GetHeight(); y++) {
    for (int x = 0; x < target.GetWidth(); x++)

      switch (target.GetCell({x, y})) {
        case CellType::EMPTY:
          printf("  ");
          break;
        case CellType::WALL:
          printf(" #");
          break;
        case CellType::START:
          printf(" S");
          break;
        case CellType::FINISH:
          printf(" F");
          break;
      }

    printf("\n");
  }
}

#endif//COA_PLANE_H

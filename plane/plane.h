//
// Created by piotr on 04/10/2021.
//

#ifndef COA_PLANE_H
#define COA_PLANE_H
#include "../clock/clock.h"
#include "../utility/coord.h"
#include "cell_type.h"
#include <ciso646>
#include <vector>
class Plane {
 public:
  Plane() : width_(0), height_(0){};
  Plane(const std::string& path){LoadFromFile(path);};
  Plane(unsigned int width, unsigned int height, int infill_percentage);
  Plane(const Plane &other);
  Plane &operator=(const Plane &other);

  CellState GetCell(const Coord &position) const;
  void SetCell(const Coord &position, CellState value);

  unsigned int GetWidth() const;
  unsigned int GetHeight() const;

  void AddBorder(CellState border_type);

  void SaveToFile(const std::string& file_name)const;
  void LoadFromFile(const std::string& file_name);

  void Clear();

 protected:
  /// x axis
  unsigned width_;
  /// y axis
  unsigned height_;

  std::vector<CellState> plane_;
};
static void ConsoleDisplay(const Plane &target) {
  if (target.GetWidth() > 30 or target.GetHeight() > 30) {

    printf("to big plane\n");
    return;
  }

  for (int y = 0; y < target.GetHeight(); y++) {
    for (int x = 0; x < target.GetWidth(); x++)

      switch (target.GetCell({x, y})) {
        case CellState::EMPTY:
          printf("  ");
          break;
        case CellState::WALL:
          printf(" #");
          break;
        case CellState::START:
          printf(" S");
          break;
        case CellState::FINISH:
          printf(" F");
          break;
      }

    printf("\n");
  }
}

static void ConsoleDisplay(const Plane &target, const std::vector<Coord> &path) {

  if (target.GetWidth() > 30 or target.GetHeight() > 30) {

    printf("to big plane\n");
    return;
  }

  for (int y = 0; y < target.GetHeight(); y++) {
    for (int x = 0; x < target.GetWidth(); x++) {

      bool belongs_to_path = false;
      for (const auto &p : path)
        if (Coord(x, y) == p and target.GetCell({x, y}) == CellState::EMPTY) {
          printf(" @");
          belongs_to_path = true;
        }
      if (not belongs_to_path)
        switch (target.GetCell({x, y})) {
          case CellState::EMPTY:
            printf("  ");
            break;
          case CellState::WALL:
            printf(" #");
            break;
          case CellState::START:
            printf(" S");
            break;
          case CellState::FINISH:
            printf(" F");
            break;
        }
    }

    printf("\n");
  }
}

#endif//COA_PLANE_H

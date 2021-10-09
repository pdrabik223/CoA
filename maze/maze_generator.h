//
// Created by piotr on 08/10/2021.
//

#ifndef COA_MAZE_MAZE_GENERATOR_H_
#define COA_MAZE_MAZE_GENERATOR_H_

#include "../plane/plane.h"
class MazeGenerator {
 public:
  MazeGenerator(const Plane &plane);

  void RecursiveDivision();
  std::pair<std::vector<Coord>,std::vector<Coord>> GenStartAndFinish(unsigned no_starts,unsigned no_finishes);

  const Plane &GetPlane() const;



 private:


 protected:

  Plane plane_;
};

#endif//COA_MAZE_MAZE_GENERATOR_H_

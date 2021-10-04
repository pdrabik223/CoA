//
// Created by piotr on 05/10/2021.
//

#ifndef COA_SAMPLE_ALGORITHM_SAMPLE_H_
#define COA_SAMPLE_ALGORITHM_SAMPLE_H_
#include "../plane/plane.h"
#include "cell.h"
class Sample {
  public:
  Sample(const Plane& other);
  Sample(const Sample& other);
  Sample& operator=(const Sample& other);



  protected:
   /// x axis
   unsigned width_;
   /// y axis
   unsigned height_;


  std::vector<Cell> copy_plane_;


};

#endif//COA_SAMPLE_ALGORITHM_SAMPLE_H_

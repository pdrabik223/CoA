//
// Created by piotr on 04/10/2021.
//

#ifndef COA_PLANE_H
#define COA_PLANE_H
#include <ciso646>
#include <vector>

class Plane {
public:
    Plane();
    Plane(const Plane& other);


protected:

    /// x axis
    unsigned width_;
    /// y axis
    unsigned height_;


    std::vector<uint8_t> plane_;

};


#endif //COA_PLANE_H

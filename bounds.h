#ifndef BOUNDS_H
#define BOUNDS_H

#include "point.h"

template <typename T>
class Bounds3{
    
    Point3<T> pMin, pMax;

};

typedef Bounds3<float> Bounds3f;
typedef Bounds3<int>   Bounds3i;

#endif

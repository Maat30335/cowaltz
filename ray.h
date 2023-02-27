#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "point.h"
#include <limits>

class Ray {
    public:
    Point3f o;
    Vector3f d;
    mutable float tMax;
    Ray() : tMax{std::numeric_limits<float>::infinity()} {};
    Ray(const Point3f &o, const Vector3f &d, float tMax = std::numeric_limits<float>::infinity())
        : o{o}, d{d}, tMax{tMax} {};
    Point3f operator()(float t) const {return o + d * t;};

};

#endif

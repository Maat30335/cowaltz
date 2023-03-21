#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "point.h"
#include <limits>

class Ray {
    public:
    Point3f o;
    Vector3f d;
    mutable double tMax;
    Ray() : tMax{std::numeric_limits<double>::infinity()} {};
    Ray(const Point3f &o, const Vector3f &d, double tMax = std::numeric_limits<double>::infinity())
        : o{o}, d{d}, tMax{tMax} {};
    Point3f operator()(double t) const {return o + d * t;};

};

#endif

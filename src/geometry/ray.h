#ifndef RAY_H
#define RAY_H

#include "vector.h"
#include "point.h"
#include <limits>
#include <iostream>

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

inline std::ostream& operator<<(std::ostream &out, const Ray &r) {
    return out << "o: " << r.o << " d: " << r.d << std::endl;
}

#endif

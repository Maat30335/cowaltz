#ifndef BOUNDS_H
#define BOUNDS_H

#include "point.h"
#include <algorithm>
#include "ray.h"
#include <limits>

template <typename T>
class Bounds3{

    public:
    Bounds3() {
    T minNum = std::numeric_limits<T>::lowest();
    T maxNum = std::numeric_limits<T>::max();
    pMin = Point3<T>(maxNum, maxNum, maxNum);
    pMax = Point3<T>(minNum, minNum, minNum);
    }
    Bounds3(const Point3<T> &p1, const Point3<T> &p2)
    : pMin(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
           std::min(p1.z, p2.z)),
      pMax(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
           std::max(p1.z, p2.z)) {
    }

    Bounds3(const Point3<T> &p) : pMin(p), pMax(p) { }
    
    Point3<T> pMin, pMax;


    inline bool IntersectP(const Ray &r, double *hitt0, double *hitt1) const{
        double t0 = 0, t1 = r.tMax;
        for(int i = 0; i < 3; ++i){
            double invDir = 1 / r.d[i];
            double tNear = (pMin[i] - r.o[i]) * invDir;
            double tFar  = (pMax[i] - r.o[i]) * invDir;
            if (tNear > tFar) std::swap(tNear, tFar);
            t0 = tNear > t0 ? tNear : t0;
            t1 = tFar  < t1 ? tFar  : t1;
            if (t0 > t1) return false;
        }
        if (hitt0) *hitt0 = t0;
        if (hitt1) *hitt1 = t1;
        return true;
    }





};

template <typename T> Bounds3<T> inline
    Union(const Bounds3<T> &b1, const Bounds3<T> &b2) {
    return Bounds3<T>(Point3<T>(std::min(b1.pMin.x, b2.pMin.x),
                                std::min(b1.pMin.y, b2.pMin.y),
                                std::min(b1.pMin.z, b2.pMin.z)),
                      Point3<T>(std::max(b1.pMax.x, b2.pMax.x),
                                std::max(b1.pMax.y, b2.pMax.y),
                                std::max(b1.pMax.z, b2.pMax.z)));
    }

template <typename T> Bounds3 <T> inline
Union(const Bounds3<T> &b, const Point3<T> &p) {
return Bounds3<T>(Point3<T>(std::min(b.pMin.x, p.x),
                                std::min(b.pMin.y, p.y),
                                std::min(b.pMin.z, p.z)),
                      Point3<T>(std::max(b.pMax.x, p.x),
                                std::max(b.pMax.y, p.y),
                                std::max(b.pMax.z, p.z)));
}

typedef Bounds3<double> Bounds3f;
typedef Bounds3<int>   Bounds3i;

#endif

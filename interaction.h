#ifndef INTERACTION_H
#define INTERACTION_H

#include "vector.h"
#include "normal.h"
#include "point.h"



struct SurfaceInteraction {
    // UV stuff with point2f
    SurfaceInteraction(Point3f p, Vector3f wo, Normal3f n) : p{p}, wo{wo}, n{n} {};
    Point3f p;
    Vector3f wo;
    Normal3f n;

};

#endif

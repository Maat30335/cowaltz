#ifndef INTERACTION_H
#define INTERACTION_H

#include "vector.h"
#include "normal.h"
#include "point.h"



struct SurfaceInteraction {
    // UV stuff with point2f
    SurfaceInteraction() {};
    SurfaceInteraction(Point3f p, Vector3f wo, Normal3f n) : p{p}, wo{wo}, n{n} {};
    Point3f p;
    Vector3f wo;
    Normal3f n; // note, this is not always normalized, it should be before shading occurs

};

#endif

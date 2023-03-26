#ifndef INTERACTION_H
#define INTERACTION_H

#include "vector.h"
#include "normal.h"
#include "point.h"

class BxDF;



struct SurfaceInteraction {
    // UV stuff with point2f
    SurfaceInteraction() {};
    SurfaceInteraction(Point3f p, Point2f uv, Vector3f dpdu, Vector3f dpdv, Vector3f wo, Normal3f n) : p{p}, uv{uv}, dpdu{dpdu}, dpdv{dpdv}, wo{wo}, n{n} {};
    Point3f p;
    Point2f uv;
    Vector3f dpdu;
    Vector3f dpdv;
    Vector3f wo;
    Normal3f n; // note, this is not always normalized, it should be before shading occurs
    BxDF *bsdf;

};

#endif

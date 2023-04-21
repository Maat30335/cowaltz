#ifndef INTERACTION_H
#define INTERACTION_H

#include "vector.h"
#include "normal.h"
#include "point.h"
#include "parameters.h"
#include "material.h"
#include "light.h"

struct SurfaceInteraction {
    // UV stuff with point2f
    SurfaceInteraction() {};
    SurfaceInteraction(Point3f p, Point2f uv, Vector3f dpdu, Vector3f dpdv, Vector3f wo, Normal3f n) : p{p}, uv{uv}, dpdu{dpdu}, dpdv{dpdv}, wo{wo}, n{n}, light{nullptr} {
        shading.n = n;
        shading.dpdu = dpdu;
        shading.dpdv = dpdv;
    };
    Point3f p;
    Point2f uv;
    Vector3f dpdu;
    Vector3f dpdv;
    Vector3f wo;
    Normal3f n; // note, this is not always normalized, it should be before shading occurs
    struct{
        Normal3f n;
        Vector3f dpdu, dpdv;
    } shading;
    PrincipledParameters parameters;
    const Light *light;

};




#endif

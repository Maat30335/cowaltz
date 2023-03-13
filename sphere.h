#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"


class Sphere : public Shape {

    public:
    Sphere(const Transform *ObjectToWorld, const Transform *WorldToObject, float radius) : Shape{ObjectToWorld, WorldToObject}, radius{radius} {};

    virtual bool Intersect(const Ray &r, float *tHit, SurfaceInteraction *isect);
    virtual Bounds3f ObjectBounds();


    public:
    float radius;




};

#endif

#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"


class Sphere : public Shape {

    public:
    Sphere(const Transform *ObjectToWorld, const Transform *WorldToObject, double radius) : Shape{ObjectToWorld, WorldToObject}, radius{radius} {};

    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const;
    // virtual Bounds3f ObjectBounds();


    public:
    double radius;




};

#endif

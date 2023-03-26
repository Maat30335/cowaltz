#ifndef SPHERE_H
#define SPHERE_H

#include "shape.h"


class Sphere : public Shape {

    public:
    Sphere(const Transform *ObjectToWorld, const Transform *WorldToObject, double radius) : Shape{ObjectToWorld, WorldToObject}, radius{radius} {};
    virtual Bounds3f ObjectBounds() const;
    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const;


    public:
    double radius;




};

#endif

#ifndef PLANE_H
#define PLANE_H

#include "shape.h"


class Plane : public Shape {
    public:
    Plane(const Transform *ObjectToWorld, const Transform *WorldToObject, double lenX, double lenZ) : Shape{ObjectToWorld, WorldToObject}, lenX{lenX}, lenZ{lenZ} {};
    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const;
    virtual Bounds3f ObjectBounds() const;
    public:
    const double lenX;
    const double lenZ;
};

#endif

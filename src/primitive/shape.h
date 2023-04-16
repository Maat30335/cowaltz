#ifndef SHAPE_H
#define SHAPE_H

#include "bounds.h"
#include "ray.h"

class SurfaceInteraction;
class Transform;

class Shape {
    public:
    Shape(const Transform *ObjectToWorld, const Transform *WorldToObject) : ObjectToWorld{ObjectToWorld}, WorldToObject{WorldToObject} {};
    // ray is in world space plz thanks
    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const = 0;
    virtual bool IntersectP(const Ray &r) const;
    virtual ~Shape(){};
    virtual Bounds3f ObjectBounds() const = 0;
    virtual Bounds3f WorldBounds() const;
    

    const Transform* ObjectToWorld;
    const Transform* WorldToObject;
};

#endif

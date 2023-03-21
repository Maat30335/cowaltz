#ifndef SHAPE_H
#define SHAPE_H

#include "transform.h"
// #include "bounds.h"
#include "ray.h"
#include "interaction.h"

class Shape {
    public:
    Shape(const Transform *ObjectToWorld, const Transform *WorldToObject) : ObjectToWorld{ObjectToWorld}, WorldToObject{WorldToObject} {};
    virtual bool Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const = 0;
    virtual ~Shape(){};
    // virtual Bounds3f ObjectBounds() const = 0;
    // virtual Bounds3f WorldBounds() const;
    

    const Transform* ObjectToWorld;
    const Transform* WorldToObject;
};

#endif

#ifndef SHAPE_H
#define SHAPE_H

#include "transform.h"
#include "bounds.h"

class Shape {
    public:
    Shape(const Transform *ObjectToWorld, const Transform *WorldToObject) : ObjectToWorld{ObjectToWorld}, WorldToObject{WorldToObject} {};
    virtual ~Shape();
    virtual Bounds3f ObjectBounds() const = 0;
    virtual Bounds3f WorldBounds() const;
    

    const Transform* ObjectToWorld;
    const Transform* WorldToObject;
};

#endif

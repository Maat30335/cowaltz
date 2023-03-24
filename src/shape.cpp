#include "shape.h"

bool Shape::IntersectP(const Ray &r) const {
    double tHit = r.tMax;
    SurfaceInteraction isect;
    return Intersect(r, &tHit, &isect);
}

Bounds3f Shape::WorldBounds() const{
    return (*ObjectToWorld)(ObjectBounds());
}

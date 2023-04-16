#include "primitive.h"
#include "interaction.h"

bool Primitive::IntersectP(const Ray &r) const{
    SurfaceInteraction isect;
    return Intersect(r, &isect);
}

bool GeoPrimitive::Intersect(const Ray &r, SurfaceInteraction *isect) const{
    if(!shape->WorldBounds().IntersectP(r, nullptr, nullptr)) return false;
    if(shape->Intersect(r, &r.tMax, isect)){
        isect->parameters = material->getSurface(isect->uv);
        return true;
    }
    return false;
}

Bounds3f GeoPrimitive::WorldBounds() const{
    return shape->WorldBounds();
}

Bounds3f PrimitiveList::WorldBounds() const{
    return bounds;
}

bool PrimitiveList::Intersect(const Ray &r, SurfaceInteraction *isect) const{
    if(!bounds.IntersectP(r, nullptr, nullptr)) return false;
    bool output = false;
    for(auto const &i : primitiveList){
        if(i->Intersect(r, isect)) output = true;
    }
    return output;
}

bool PrimitiveList::IntersectP(const Ray &r) const{
    if(!bounds.IntersectP(r, nullptr, nullptr)) return false;
    for(auto const &i : primitiveList){
        if(i->IntersectP(r)) return true;
    }
    return false;
}

void PrimitiveList::addPrim(std::shared_ptr<Primitive> primitive){
    primitiveList.push_back(primitive);
    bounds = Union(bounds, primitive->WorldBounds());
}

void PrimitiveList::clearPrims(){
    primitiveList.clear();
    Bounds3f newBound;
    bounds = newBound;
}

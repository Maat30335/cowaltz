#include "primitive.h"


bool GeoPrimitive::Intersect(const Ray &r, SurfaceInteraction *isect){
    return shape->Intersect(r, &r.tMax, isect);
    // we will add bounds stuff one day
}

bool PrimitiveList::Intersect(const Ray &r, SurfaceInteraction *isect){
    bool output = false;
    for(auto const &i : primitiveList){
        if(i->Intersect(r, isect)) output = true;
    }
    return output;
}

void PrimitiveList::addPrim(std::shared_ptr<Primitive> primitive){
    primitiveList.push_back(primitive);
}

void PrimitiveList::clearPrims(){
    primitiveList.clear();
}

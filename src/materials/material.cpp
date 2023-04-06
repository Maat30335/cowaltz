#include "material.h"

PrincipledParameters ConstantMaterial::getSurface(const Point2f &uv) const{
    return surface;
}

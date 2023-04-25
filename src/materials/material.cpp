#include "material.h"
#include "texture.h"

PrincipledParameters ConstantMaterial::getSurface(const Point2f &uv) const{
    return surface;
}

PrincipledParameters TestMaterial::getSurface(const Point2f &uv) const{
    PrincipledParameters output{Color(uv.x, uv.y, 0), 1, 1.5, 0, 0};
    return output;
}

PrincipledParameters ColorMaterial::getSurface(const Point2f &uv) const{
    PrincipledParameters output{baseColor->getColor(uv), roughness, IOR, metallic, specTrans};
    return output;
}

PrincipledParameters PrincipledMaterial::getSurface(const Point2f &uv) const{
    PrincipledParameters output{baseColor->getColor(uv), roughness->getColor(uv).r, IOR, metallic->getColor(uv).r, specTrans->getColor(uv).r};
    Color normal = (2 * normalMap->getColor(uv)) + (-1.);
    output.shadingN = Normalize(Normal3f(normal.r, normal.b, normal.g)); // change back plz (rbg)
    return output;
}

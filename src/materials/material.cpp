#include "material.h"

PrincipledShader::PrincipledShader(Color baseColor, double roughness, double IOR){
    bsdf.reset();
    bsdf = std::make_unique<DisneyBSDF>(baseColor, roughness, IOR);
}

void PrincipledShader::UpdateBSDF(SurfaceInteraction *isect){
    isect->bsdf = bsdf.get();
}

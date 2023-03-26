#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "bsdf.h"
#include "disney.h"


class Material {
    public:
    virtual void UpdateBSDF(SurfaceInteraction *isect) = 0;
};

class PrincipledShader : public Material {
    public:
    PrincipledShader(Color baseColor, double roughness, double IOR);
    void UpdateBSDF(SurfaceInteraction *isect);
    private:
    std::unique_ptr<DisneyBSDF> bsdf;
};



#endif

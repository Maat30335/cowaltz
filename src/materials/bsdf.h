#ifndef BSDF_H
#define BSDF_H

#include "spectrum.h"
#include "vector.h"
#include "interaction.h"


class BxDF {

    public:
    virtual Color f(const Vector3f &wi, const Vector3f &wo, const SurfaceInteraction &isect) const = 0;
    virtual ~BxDF(){};
    // add sampling function later

};





#endif

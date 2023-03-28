#ifndef DISNEY_H
#define DISNEY_H

#include "bsdf.h"
#include "spectrum.h"
#include "interaction.h"

struct PrincipledParameters {
    Color baseColor;
    double roughness;
    double IOR;
};


class DisneyBSDF : public BxDF {
    public:
    DisneyBSDF(const Color &baseColor, double roughness, double IOR) : parameters{baseColor, roughness, IOR} {};
    virtual Color f(const Vector3f &wi, const Vector3f &wo, const SurfaceInteraction &isect) const;
    PrincipledParameters parameters;

    private:
    Color EvaluateDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n) const;
    Color EvaluateSpecularBRDF(const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n) const;
    double GTR2(const Vector3f &wh, const Normal3f &n) const;
    double GeoMasking(const Vector3f &wi, const Vector3f &wo, const Normal3f &n) const;
    double Fresnel(const Vector3f &wh, const Vector3f &wo) const;
    
};




#endif

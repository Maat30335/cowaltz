#include "disney.h"
#include "useful.h"

Color DisneyBSDF::f(const Vector3f &wi, const Vector3f &wo, const SurfaceInteraction &isect) const{
    //temp

    // half vector
    
    Vector3f wh = (wi + wo) / (wi + wo).Norm();

    // at this point all vectors (and normal) should be normalized thanks

    return EvaluateDiffuse(wi, wo, wh, isect.n);
}

Color DisneyBSDF::EvaluateDiffuse(const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n) const{
    Color fLambert = parameters.baseColor / Pi; // this could change later

    double cosL = AbsDot(wi, n);
    double cosV = AbsDot(wo, n);
    double cosD = AbsDot(wi, wh);

    double fL = pow(1 - cosL, 5);

    double fV = pow(1 - cosV, 5);

    double rR = 2 * parameters.roughness * cosD * cosD;

    Color fRetro = (parameters.baseColor / Pi) * rR * (fL + fV + fL * fV * (rR - 1));

    Color fDiffuse = fLambert * (1 - 0.5 * fL) * (1 - 0.5 * fV) + fRetro;

    return fDiffuse;
}



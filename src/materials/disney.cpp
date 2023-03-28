#include "disney.h"
#include "useful.h"

Color DisneyBSDF::f(const Vector3f &wi, const Vector3f &wo, const SurfaceInteraction &isect) const{
    //temp

    // half vector
    
    Vector3f wh = (wi + wo) / (wi + wo).Norm();


    // at this point all vectors (and normal) should be normalized thanks

    return EvaluateDiffuse(wi, wo, wh, isect.n) + GTR2(wh, isect.n) * GeoMasking(wi, wo, isect.n) * Fresnel(wh, wo);
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


double DisneyBSDF::GTR2(const Vector3f &wh, const Normal3f &n) const{
    // Trowbridge-Reitz distribution
    double alpha = parameters.roughness * parameters.roughness;
    return (alpha * alpha) / (Pi * pow(pow(AbsDot(n, wh), 2) * (alpha * alpha - 1) + 1, 2));
}


double DisneyBSDF::GeoMasking(const Vector3f &wi, const Vector3f &wo, const Normal3f &n) const{
    double alpha = parameters.roughness * parameters.roughness;

    double lTerm = AbsDot(n, wi) * std::sqrt(pow(Dot(wo, n), 2) * (1 - alpha * alpha) + alpha * alpha);
    double rTerm = AbsDot(n, wo) * std::sqrt(pow(Dot(wi, n), 2) * (1 - alpha * alpha) + alpha * alpha);

    return 0.5 / (lTerm + rTerm);
}

double DisneyBSDF::Fresnel(const Vector3f &wh, const Vector3f &wo) const{
    double f0 = pow((parameters.IOR - 1) / (parameters.IOR + 1), 2); 
    return f0 + (1 - f0) * pow((1 - AbsDot(wh, wo)), 5);
}





#ifndef DISNEY_H
#define DISNEY_H

#include "spectrum.h"
#include "interaction.h"
#include "parameters.h"
#include "material.h"



struct BSDFSample {
    bool isBlack = false;
    bool hitIn = false;
    Color reflectance;
    Vector3f wi;
};

namespace Disney {
    Color EvaluateDiffuse(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n);
    double GTR2(const PrincipledParameters &parameters, const Vector3f &wh, const Normal3f &n);
    double G1(const Vector3f &w, const Vector3f &wh,const Normal3f &n, double alpha);
    double SmithGGX(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n);
    Color SchlickFresnel(const PrincipledParameters &parameters, const Vector3f &wh, const Vector3f &wo, const Vector3f &wi);
    Color EvaluateSpecBRDF(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wh, const Normal3f &n);
    Color EvaluateSpecTransmission(const PrincipledParameters &parameters, const Vector3f &wi, const Vector3f &wo, const Vector3f &wt, const Normal3f &n);
    void CalculateLobePDF(const PrincipledParameters &parameters, double &pSpecular, double &pDiffuse, double &pSpecTrans);
    BSDFSample SampleSpecBRDF(const PrincipledParameters &parameters, const Vector3f &wo);
    BSDFSample SampleDiffuse(const PrincipledParameters &parameters, const Vector3f &wo);
    BSDFSample SampleSpecTrans(const PrincipledParameters &parameters, const Vector3f &wo);
    BSDFSample SampleDisney(const SurfaceInteraction &isect, const Vector3f &v);

};





#endif

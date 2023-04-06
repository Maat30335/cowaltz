#ifdef PARAMETERS_H
#define PARAMETERS_H

#include "spectrum.h"


struct PrincipledParameters {
    
    PrincipledParameters(Color baseColor, double roughness, double IOR, double metallic, double specTrans) : 
                        baseColor{baseColor}, roughness{roughness}, IOR{IOR}, metallic{metallic}, specTrans{specTrans} {};
    
    Color baseColor;
    double roughness;
    double IOR;
    double metallic;
    double specTrans;
};



#endif

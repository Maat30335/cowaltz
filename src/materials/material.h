#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "point.h"
#include "spectrum.h"

struct PrincipledParameters {
    PrincipledParameters() {};
    
    PrincipledParameters(Color baseColor, double roughness, double IOR, double metallic, double specTrans) : 
                        baseColor{baseColor}, roughness{roughness}, IOR{IOR}, metallic{metallic}, specTrans{specTrans} {};
    
    Color baseColor;
    double roughness;
    double IOR;
    double metallic;
    double specTrans;
};

class Material {
    public:
    virtual PrincipledParameters getSurface(const Point2f &uv) const = 0;
    virtual ~Material() {};
};

class ConstantMaterial : public Material {
    public:
    ConstantMaterial(PrincipledParameters surface) : surface{surface} {};
    virtual PrincipledParameters getSurface(const Point2f &uv) const;
    private:
    PrincipledParameters surface;
};




#endif

#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>
#include "point.h"
#include "spectrum.h"

class Texture;

struct PrincipledParameters {
    PrincipledParameters() {};
    
    PrincipledParameters(Color baseColor, double roughness, double IOR, double metallic, double specTrans) : 
                        baseColor{baseColor}, roughness{roughness}, IOR{IOR}, metallic{metallic}, specTrans{specTrans}, shadingN{Normal3f(0, 1, 0)} {};
    
    Color baseColor;
    double roughness;
    double IOR;
    double metallic;
    double specTrans;
    Normal3f shadingN;
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

class TestMaterial : public Material {
    public:
    virtual PrincipledParameters getSurface(const Point2f &uv) const;
};

class ColorMaterial : public Material {
    public:
    virtual PrincipledParameters getSurface(const Point2f &uv) const;
    ColorMaterial(const Texture* baseColor, double roughness, double IOR, double metallic, double specTrans) :
                    baseColor{baseColor}, roughness{roughness}, IOR{IOR}, metallic{metallic}, specTrans{specTrans} {};
    private:
    const Texture* baseColor;
    double roughness;
    double IOR;
    double metallic;
    double specTrans;
};

class PrincipledMaterial : public Material {
    public:
    virtual PrincipledParameters getSurface(const Point2f &uv) const;
    PrincipledMaterial(const Texture* baseColor, const Texture* roughness, double IOR, const Texture* metallic, const Texture* specTrans, const Texture* normalMap) :
                        baseColor{baseColor}, roughness{roughness}, IOR{IOR}, metallic{metallic}, specTrans{specTrans}, normalMap{normalMap} {};
    private:
    const Texture* baseColor;
    const Texture* roughness;
    double IOR;
    const Texture* metallic;
    const Texture* specTrans;
    const Texture* normalMap;
};


#endif

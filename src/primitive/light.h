#ifndef LIGHT_H
#define LIGHT_H

#include "primitive.h"
#include <memory>
#include "plane.h"

struct LightSample {
    bool isBlack;
    double tHit;
    Vector3f wi;
    Color emittance;
    double pdf;
};

class Light : public Primitive {
    public:
    // note we are not using the normal to do the dot thingy, the bsdf will do that
    Light(double intensity, Color lightColor) : intensity{intensity}, lightColor{lightColor} {};
    virtual LightSample Sample_Li(const Point3f &p) const = 0;
    virtual double pdf(const Vector3f &wi, const Point3f &p, const Point3f &pHit) const = 0;
    virtual Color getEmittance() const;
    protected:
    double intensity;
    Color lightColor;
};

class RectangleLight : public Light {
    public:
    RectangleLight(const Transform *ObjectToWorld, const Transform *WorldToObject, double lenX, double lenZ, double intensity, Color lightColor);
    virtual bool Intersect(const Ray &r, SurfaceInteraction *isect) const;
    virtual LightSample Sample_Li(const Point3f &p) const;
    virtual double pdf(const Vector3f &wi, const Point3f &p, const Point3f &pHit) const;
    virtual Bounds3f WorldBounds() const;
    std::unique_ptr<Plane> mesh;
};





#endif


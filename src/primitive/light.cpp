#include "light.h"
#include "useful.h"
#include "interaction.h"
#include "transform.h"
#include <iostream>

Color Light::getEmittance() const{
    return intensity * lightColor;
}


RectangleLight::RectangleLight(const Transform *ObjectToWorld, const Transform *WorldToObject, double lenX, double lenZ, double intensity, Color lightColor) : 
    Light{intensity, lightColor} {
    mesh = std::make_unique<Plane>(ObjectToWorld, WorldToObject, lenX, lenZ);
}

bool RectangleLight::Intersect(const Ray &r, SurfaceInteraction *isect) const{
    if(!mesh->Intersect(r, &r.tMax, isect)) return false;
    isect->parameters = PrincipledParameters(lightColor, 1, 1.5, 0, 0);
    isect->light = static_cast<const Light*>(this);
    return true;
}

Bounds3f RectangleLight::WorldBounds() const{
    return mesh->WorldBounds();
}



LightSample RectangleLight::Sample_Li(const Point3f &p) const{
    LightSample sample;
    Point3f pHit = Point3f(random_double() * mesh->lenX, 0, random_double() * mesh->lenZ); // in object space
    Vector3f wi = pHit - (*(mesh->WorldToObject))(p);
    sample.wi = (*(mesh->ObjectToWorld))(pHit) - p;

    if(wi.y >= 0){
        sample.isBlack = true;
        return sample;
    }

    sample.isBlack = false;
    // move sample wi down here

    sample.tHit = sample.wi.Norm();
    sample.wi /= sample.tHit;
    sample.emittance = intensity * lightColor;
    sample.pdf = pdf(sample.wi, p, (*(mesh->ObjectToWorld))(pHit));

    return sample;
}

double RectangleLight::pdf(const Vector3f &wi, const Point3f &p, const Point3f &pHit) const{
    double pdf = DistanceSquared(p, pHit) / (std::abs(Normalize((*(mesh->WorldToObject))(wi)).y) * mesh->lenX * mesh->lenZ);
    return pdf;
}

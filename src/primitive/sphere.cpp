#include "sphere.h"
#include "useful.h"
#include "transform.h"


bool Sphere::Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const{
    Ray ray = (*WorldToObject)(r);
    double a = ray.d.x * ray.d.x + ray.d.y * ray.d.y + ray.d.z * ray.d.z;
    double b = 2 * (ray.d.x * ray.o.x + ray.d.y * ray.o.y + ray.d.z * ray.o.z);
    double c = ray.o.x * ray.o.x + ray.o.y * ray.o.y + ray.o.z * ray.o.z - radius * radius;
    
    double t0, t1;
    if (!Quadratic(a, b, c, &t0, &t1)) return false;

    double tTemp = t0;

    if(tTemp > ray.tMax) return false; // should these still be tMax?

    if(tTemp < 0) tTemp = t1;

    if(tTemp < 0 || tTemp > ray.tMax) return false;

    // We hit ray, compute stuff

    Point3f pHit = ray(tTemp);
    *tHit = tTemp;

    double phi = std::atan2(pHit.y, pHit.x);
    if(phi < 0) phi += 2 * Pi;
    double theta = std::acos(pHit.z / radius);

    Point2f uv{phi / (2 * Pi), theta / Pi};

    // calc 3 pain

    Vector3f dpdu{-2 * Pi * pHit.y, 2 * Pi * pHit.x, 0};
    double zRadius = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
    double invZRadius = 1 / zRadius;
    double cosPhi = pHit.x * invZRadius;
    double sinPhi = pHit.y * invZRadius;
    Vector3f dpdv = Pi * Vector3f(pHit.z * cosPhi, pHit.z * sinPhi, -radius * std::sin(theta));

    *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, uv, dpdu, dpdv, -ray.d, (Normal3f)pHit / radius));
    return true;

}

Bounds3f Sphere::ObjectBounds() const{
    return Bounds3f(Point3f(-radius, -radius, -radius), Point3f(radius, radius, radius));
}

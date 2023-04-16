#include "plane.h"
#include "transform.h"
#include "useful.h"

bool Plane::Intersect(const Ray &r, double *tHit, SurfaceInteraction *isect) const{
    Ray ray = (*WorldToObject)(r);

    if(ray.d.y >= 0. && ray.o.y >= 0.) return false;
    if(ray.d.y <= 0. && ray.o.y <= 0.) return false;

    double tTemp = -ray.o.y / ray.d.y;

    if(tTemp > *tHit) return false;

    Point3f pHit = ray(tTemp);

    if(pHit.x < 0 || pHit.x > lenX) return false;
    if(pHit.z < 0 || pHit.z > lenZ) return false;

    *tHit = tTemp;
    Point2f uv{pHit.x - (int)pHit.x, pHit.z - (int)pHit.z};

    Vector3f dpdu{1, 0, 0};
    Vector3f dpdv{0, 0, 1};
    Normal3f n{0, 1, 0};

    *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, uv, dpdu, dpdv, -ray.d, n));
    return true;


}

Bounds3f Plane::ObjectBounds() const{
    return Bounds3f(Point3f(0, -0.001, 0), Point3f(lenX, 0.001, lenZ));
}


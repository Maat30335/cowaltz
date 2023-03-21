#include "sphere.h"
#include "useful.h"


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


    Point3f pHit = ray(tTemp);
    *tHit = tTemp;
    *isect = (*ObjectToWorld)(SurfaceInteraction(pHit, -ray.d, (Normal3f)pHit));
    return true;

}

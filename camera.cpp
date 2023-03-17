#include "camera.h"

Ray PerspectiveCamera::GenerateRay(const Point2f &sample) const{
    float HeightRatio = resolution.y / (float)resolution.x;
    Vector3f d = Vector3f(sample.x - 0.5f, (sample.y - 0.5f) * HeightRatio, -fov);
    Ray r = Ray(Point3f(0., 0., 0.), d);
    return ((*CameraToWorld)(r));
}

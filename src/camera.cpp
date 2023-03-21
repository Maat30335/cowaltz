#include "camera.h"
#include <iostream>

PerspectiveCamera::PerspectiveCamera(const Transform *CameraToWorld, const Point2i &resolution, double fov) : 
    Camera{CameraToWorld, resolution}, fov{fov}
    {
    lowerLeft.x = 0 - (double)resolution.x / 2;
    lowerLeft.y = 0 - (double)resolution.y / 2;
}

Ray PerspectiveCamera::GenerateRay(const Point2f &sample, const Point2i &pixel) const{ // we need to change this to base it off pixel AND sample thanks
    Point2f p = (lowerLeft + (Point2f)pixel + sample);
    // std::cout << "Point2f: " << p.x << ", " << p.y << std::endl;
    Vector3f d = Vector3f(p.x, p.y, -fov);
    Ray r{Point3f(0, 0, 0), d};
    return ((*CameraToWorld)(r));
}

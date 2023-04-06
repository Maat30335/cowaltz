#include "camera.h"
#include <iostream>
#include <cmath>
#include "useful.h"

PerspectiveCamera::PerspectiveCamera(const Transform *CameraToWorld, const Point2i &resolution, double fov, double focusPlane, double aperture) : 
    Camera{CameraToWorld, resolution}, fov{fov}, focusPlane{focusPlane}, aperture{aperture}
    {
    double h = std::tan(fov / 2.) * 2 * focusPlane;

    increment = h / resolution.x;

    double aspectRatio = (double)resolution.y / (double)resolution.x;
    lowerLeft.x = 0. - h / 2.;
    lowerLeft.y = 0. - h * aspectRatio / 2.;
}

Ray PerspectiveCamera::GenerateRay(const Point2f &sample, const Point2i &pixel) const{ // we need to change this to base it off pixel AND sample thanks
    Point2f p = (lowerLeft + ((Point2f)pixel + sample) * increment);
    // std::cout << "Point2f: " << p.x << ", " << p.y << std::endl;
    Point2f origin2D = aperture * randomInCircle(Point2f(random_double(), random_double()));
    Point3f origin3D = Point3f(origin2D.x, origin2D.y, 0);

    Ray r{origin3D, Point3f(p.x, p.y, -focusPlane) - origin3D};
    return ((*CameraToWorld)(r));
}

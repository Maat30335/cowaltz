#ifndef CAMERA_H
#define CAMERA_H

#include "transform.h"
#include "point.h"

class Camera {
    public:
    Camera(const Transform *CameraToWorld, const Point2i &resolution) : CameraToWorld{CameraToWorld}, resolution{resolution} {}; 
    const Transform *CameraToWorld;
    const Point2i resolution;

    virtual Ray GenerateRay(const Point2f &sample, const Point2i &pixel) const = 0; // ray should be returned in world space plz    
};

class PerspectiveCamera : public Camera {
    public:
    PerspectiveCamera(const Transform *CameraToWorld, const Point2i &resolution, double fov, double focusPlane, double aperture); // we add fov later
    virtual Ray GenerateRay(const Point2f &sample, const Point2i &pixel) const;
    
    private:
    double fov;
    double focusPlane;
    double aperture;
    Point2f lowerLeft;
    double increment;
};


#endif
